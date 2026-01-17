#include "Graphics.h"

#include <iostream>

#include "Camera.h"
#include "Mesh.h"
#include "Shaders.h"
#include "stb_image.h"
#include "../core/Debug.h"

namespace gl {

    ShaderProgram* Graphics::active_shader_;
    ShaderProgram Graphics::phong_;
    ShaderProgram Graphics::skinned_;
    std::unordered_map<std::string, DrawShape> Graphics::shapes_;

    void Graphics::initialize() {
        initializePhongShader();
        setAmbientLight(glm::vec3(1.f));
    }

    void Graphics::tearDown() {
        phong_.deleteProgram();
    }

    void Graphics::usePhongShader() {
        phong_.use();
        active_shader_ = &phong_;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        // glEnable(GL_POLYGON_SMOOTH);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonOffset(1.0, 1.0);
    }

    void Graphics::useSkinnedShader() {
        skinned_.use();
        active_shader_ = &skinned_;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        // glEnable(GL_POLYGON_SMOOTH);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPolygonOffset(1.0, 1.0);
    }

    void Graphics::drawObject(const DrawShape* drawShape, const Transform& transform, const DrawMaterial& material) {
        const auto model_matrix = transform.getModelMatrix();
        phong_.setMat4("model", model_matrix);
        phong_.setMat3("normal", glm::transpose(glm::inverse(glm::mat3(model_matrix))));

        setMaterialUniforms(material);

        glBindVertexArray(drawShape->vao);
        glDrawElements(GL_TRIANGLES, 3 * drawShape->numTriangles, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    void Graphics::drawMesh(const DrawMesh* draw_mesh, const Transform& transform) {
        const auto model_matrix = transform.getModelMatrix();
        phong_.setMat4("model", model_matrix);
        phong_.setMat3("normal", glm::transpose(glm::inverse(glm::mat3(model_matrix))));

        for (const auto& obj : draw_mesh->objects) {
            // std::cout << "Material info: " << std::endl;
            // std::cout << "Ambient: " << obj.material.ambient.r << ", " << obj.material.ambient.g << ", " << obj.material.ambient.b << std::endl;
            // std::cout << "Diffuse: " << obj.material.diffuse.r << ", " << obj.material.diffuse.g << ", " << obj.material.diffuse.b << std::endl;
            // std::cout << "Specular: " << obj.material.specular.r << ", " << obj.material.specular.g << ", " << obj.material.specular.b << std::endl;
            // std::cout << "Shininess: " << obj.material.shininess << std::endl;
            // std::cout << "Opacity: " << obj.material.opacity << std::endl;
            setMaterialUniforms(obj.material);
            glBindVertexArray(obj.shape.vao);
            glDrawElements(GL_TRIANGLES, 3 * obj.shape.numTriangles, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

    }


    void Graphics::addShape(const char* name, const DrawShape& shape) {
        shapes_[name] = shape;
    }



    const DrawShape* Graphics::getShape(const std::string& shape_name) {
        return &shapes_[shape_name];
    }


    void Graphics::setCameraUniforms(const Camera* camera) {
        active_shader_->setMat4("view", camera->getViewMatrix());
        active_shader_->setMat4("projection", camera->getProjection());
        active_shader_->setVec3("camera_pos", camera->getPosition());
    }

    void Graphics::setLight(const Light& light) {
        active_shader_->setVec3("light_position", light.position);
        active_shader_->setVec3("light_color", light.color);
    }

    void Graphics::setAmbientLight(const glm::vec3& ambient) {
        usePhongShader();
        active_shader_->setVec3("ambient_light", ambient);
        useSkinnedShader();
        active_shader_->setVec3("ambient_light", ambient);
    }

    void Graphics::initializePhongShader() {
        const auto frag = "Resources/Shaders/phong_frag.glsl";
        const auto phong_vert = "Resources/Shaders/phong_vert.glsl";
        phong_ = Shaders::createShaderProgram(phong_vert,frag);//ShaderProgram(vert, frag);

        const auto skinned_vert = "Resources/Shaders/skinned_vert.glsl";
        skinned_ = Shaders::createShaderProgram(skinned_vert,phong_.getFragmentID());

        active_shader_= &phong_;
    }

    void Graphics::setMaterialUniforms(const DrawMaterial& material) {
        active_shader_->setVec3("ambient", material.ambient);
        active_shader_->setVec3("diffuse", material.diffuse);
        active_shader_->setVec3("specular", material.specular);
        active_shader_->setFloat("shininess", material.shininess);
        active_shader_->setFloat("opacity", material.opacity);
        bindMaterialTextures(material.textures);
    }

    void Graphics::bindMaterialTextures(const Textures& textures) {
        active_shader_->setInt("texture_flags", textures.flags);

        if (textures.ambient != 0) {
            bindTexture(textures.ambient, 0, "texture_ambient");
        }
        if (textures.diffuse != 0) {
            bindTexture(textures.diffuse, 1, "texture_diffuse");
        }
        if (textures.specular != 0) {
            bindTexture(textures.specular, 2, "texture_specular");
        }

    }

    void Graphics::bindTexture(const GLuint texture, const int unit, const char* uniform_name) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture);
        active_shader_->setInt(uniform_name, unit); // sampler2D bound by binding the texture unit to the uniform
    }

    void Graphics::drawLine3D(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color, float width) {
        // Create line vertices
        float vertices[] = {
            start.x, start.y, start.z,
            end.x, end.y, end.z
        };

        // Create and bind VAO/VBO
        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Set line width
        glLineWidth(width);

        // Set material to use the color (using emissive-like approach)
        DrawMaterial lineMaterial;
        lineMaterial.ambient = color;
        lineMaterial.diffuse = color;
        lineMaterial.specular = glm::vec3(0.0f);
        lineMaterial.shininess = 1.0f;
        lineMaterial.opacity = 1.0f;

        setMaterialUniforms(lineMaterial);

        // Set identity transform (vertices are already in world space)
        active_shader_->setMat4("model", glm::mat4(1.0f));
        active_shader_->setMat3("normal", glm::mat3(1.0f));

        // Draw the line
        glDrawArrays(GL_LINES, 0, 2);

        // Cleanup
        glBindVertexArray(0);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);

        // Reset line width to default
        glLineWidth(1.0f);
    }
}


