#pragma once
#include "GL/glew.h"


struct aiMaterial;
struct aiScene;
struct aiTexture;

namespace gl {
    class Primitive;
    struct DrawShape;

    struct DrawMaterial {

        // Color
        glm::vec3 ambient = glm::vec3(1.f);
        glm::vec3 diffuse = glm::vec3(1.f);
        glm::vec3 specular = glm::vec3(1.f);
        float shininess = 1.0f;
        float opacity = 1.0f;
    };

    static const DrawMaterial defaultMaterial = {
        .ambient = glm::vec3(1.f, 0.f, 1.f),
        .diffuse = glm::vec3(0.8f, 0.0f, 0.8f),
        .specular = glm::vec3(1.0f, 0.5f, 1.0f),
        .shininess = 100.0f,
        .opacity = 1.0f,
    };

    struct DrawShape {
        GLuint vao = 0;
        GLuint vbo = 0; // vertex buffer id
        GLuint ebo = 0; // element buffer id (for indexed rendering)
        size_t numTriangles = 0;
        glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
        glm::vec3 max = glm::vec3(std::numeric_limits<float>::lowest());
    };


    struct DrawObject {
        DrawShape shape;
        DrawMaterial material = defaultMaterial;
    };

    struct DrawMesh {
        std::vector<DrawObject> objects;
        glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
        glm::vec3 max = glm::vec3(std::numeric_limits<float>::lowest());
    };

    class Mesh {
    public:

        static DrawShape loadStaticShapeIndexed(const std::vector<float>& buffer_data, const std::vector<unsigned int>& indices);
        static DrawShape loadPrimitive(const Primitive& primitive);
        static DrawMesh loadStaticMesh(const char* filename);

        static std::vector<glm::vec3> loadVertices(const char* filename);

    private:


    };
}
