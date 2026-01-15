// #ifndef VIEWER_VISUALIZER_H
// #define VIEWER_VISUALIZER_H
//
// #include <GL/glew.h>
// #include <glm/gtc/type_ptr.hpp>
// #include "collision.h"
// #include "../mesh.h"
//
//
// class Visualizer {
// public:
//     void init();
//     void drawMTV(GLuint shaderProgram, const Collision& collision);
//     void drawCollider(GLuint shaderProgram, const Collision& collision);
// private:
//     bool hasMTV(const Collision& collision);
//     static void drawVector(glm::vec3 start, glm::vec3 end, glm::vec3 color);
//     void drawObject(GLuint shaderProgram, gl::DataTex data, glm::vec3 pos);
//
//     gl::DataTex m_ellipsoid;
//     gl::DataTex m_cylinder1;
//     gl::DataTex m_cylinder2;
// };
//
//
// #endif //VIEWER_VISUALIZER_H
