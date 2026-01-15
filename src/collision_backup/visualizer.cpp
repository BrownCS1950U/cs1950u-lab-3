// #include "visualizer.h"
// #include "../camera.h"
//
// #define COLOR glm::vec3(1.0f, 0.0f, 0.0f) // Red
//
// void Visualizer::init() {
//     m_ellipsoid = gl::Mesh::load_obj("../data/ellipsoid.obj");
//     m_cylinder1 = gl::Mesh::load_obj("../data/cylinder.obj");
//     m_cylinder2 = gl::Mesh::load_obj("../data/cylinder.obj");
// }
//
// bool Visualizer::hasMTV(const Collision& collision) {
//     std::vector<Collider> colliders = collision.getColliders();
//
//     for (const Collider& collider : colliders) {
//         if (collision.getMtv(collider.id) != glm::vec3(0.0f)) {
//             return true;
//         }
//     }
//     return false;
// }
//
// void Visualizer::drawMTV(GLuint shaderProgram, const Collision& collision) {
//     if (!hasMTV(collision)) {
//         return;
//     }
//
//     std::vector<Collider> colliders = collision.getColliders();
//
//     glm::mat4 view = gl::Camera::getViewMatrix();
//     glm::mat4 proj = gl::Camera::getProjection(1920.0f / 1080.0f);
//
//     glUseProgram(shaderProgram);
//     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uModelView"),
//                        1, GL_FALSE, glm::value_ptr(view));
//     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjection"),
//                        1, GL_FALSE, glm::value_ptr(proj));
//
//     for (const Collider& collider : colliders) {
//         glm::vec3 mtv = collision.getMtv(collider.id);
//         if (mtv != glm::vec3(0.0f)) {
//             drawVector(collider.pos, mtv, COLOR);
//         }
//     }
// }
//
// void Visualizer::drawCollider(GLuint shaderProgram, const Collision& collision) {
//     std::vector<Collider> colliders = collision.getColliders();
//     if (colliders.empty()) {
//         return;
//     }
//
//     glUseProgram(shaderProgram);
//     if (collision.getMode() == Ellipsoid_Triangle) {
//         drawObject(shaderProgram, m_ellipsoid, colliders[0].pos);
//         drawObject(shaderProgram, m_cylinder1, colliders[2].pos);
//         drawObject(shaderProgram, m_cylinder1, colliders[4].pos);
//     } else if (collision.getMode() == GJK_EPA) {
//         drawObject(shaderProgram, m_ellipsoid, colliders[1].pos);
//         drawObject(shaderProgram, m_cylinder1, colliders[3].pos);
//         drawObject(shaderProgram, m_cylinder1, colliders[5].pos);
//     }
// }
//
// void Visualizer::drawObject(GLuint shaderProgram, gl::DataTex data, glm::vec3 pos) {
//     glm::mat4 view = gl::Camera::getViewMatrix();
//     glm::mat4 proj = gl::Camera::getProjection(1920.0f / 1080.0f);
//     glm::mat4 model = glm::translate(glm::mat4(1.0f), pos) * glm::mat4(1.0f);
//     glm::mat4 MVP = proj * view * model;
//
//     // Send MVP to shader
//     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uMVP"),
//                        1, GL_FALSE, glm::value_ptr(MVP));
//
//     gl::Mesh::draw(GL_FRONT_AND_BACK, GL_FILL, shaderProgram, data);
// }
//
// void Visualizer::drawVector(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
//     GLuint vectorVAO, vectorVBO;
//     glGenVertexArrays(1, &vectorVAO);
//     glGenBuffers(1, &vectorVBO);
//
//     glBindVertexArray(vectorVAO);
//     glBindBuffer(GL_ARRAY_BUFFER, vectorVBO);
//
//     GLfloat vectorVertices[] = {
//             // Position (xyz)   // Color (rgb)
//             start[0], start[1], start[2], color[0], color[1], color[2],
//             end[0],   end[1],   end[2],   color[0], color[1], color[2]
//     };
//
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vectorVertices), vectorVertices, GL_STATIC_DRAW);
//
//     // Position attribute
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);
//
//     // Color attribute
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//     glEnableVertexAttribArray(1);
//
//     // Draw the vector
//     glLineWidth(10.0f);
//     glDrawArrays(GL_LINES, 0, 2);
//
//     // Cleanup
//     glBindVertexArray(0);
//     glDeleteVertexArrays(1, &vectorVAO);
//     glDeleteBuffers(1, &vectorVBO);
// }
//
