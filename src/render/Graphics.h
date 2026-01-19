#pragma once

#include "Mesh.h"
#include "../Transform.h"


namespace gl {
    struct SkinnedMesh;
    class Camera;
    class ShaderProgram;





    struct Light {
        glm::vec3 position = glm::vec3(0,0,0);
        glm::vec3 color = glm::vec3(1.f,1.f,1.f);
    };


    class Graphics {
    public:
        static void initialize();
        static void tearDown();

        static void usePhongShader();
        static void useSkinnedShader();
        static void setCameraUniforms(const Camera* camera);
        static void setLight(const Light& light);
        static void setAmbientLight(const glm::vec3& ambient);


        static void drawObject(const DrawShape* drawShape, const Transform& transform, const DrawMaterial& material = defaultMaterial);
        static void drawMesh(const DrawMesh* draw_mesh, const Transform& transform);

        // Debug line drawing
        static void drawLine3D(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color, float width = 1.0f);

        static void addShape(const char* name, const DrawShape& shape);
        static const DrawShape* getShape(const std::string& shape_name);

    private:
        static void initializePhongShader();
        static void setMaterialUniforms(const DrawMaterial& material);

        static std::unordered_map<std::string, DrawShape> shapes_;

        static ShaderProgram* active_shader_;
        static ShaderProgram phong_;
        static ShaderProgram skinned_;

    };
}

