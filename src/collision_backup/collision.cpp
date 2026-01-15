// #include "collision.h"
// #include "GLFW/glfw3.h"
//
// Collision::Collision() :
//     m_mesh("../data/playground.obj") {
//
//     // Map to m_ellipsoid in visualizer
//     addEllipsoidCollider(glm::vec3(1, 4, -10), glm::vec3(2, 4, 2)); // id 0
//     addGJKCollider(glm::vec3(1, 4, -10), "../data/ellipsoid.obj");  // id 1
//
//     // Map to m_cylinder1 in visualizer
//     addCylinderCollider(glm::vec3(6, 2, 3), 2, 4);                  // id 2
//     addGJKCollider(glm::vec3(6, 2, 3), "../data/cylinder.obj");    // id 3
//
//     // Map to m_cylinder2 in visualizer
//     addCylinderCollider(glm::vec3(-8, 2, -16), 2, 4);               // id 4
//     addGJKCollider(glm::vec3(-8, 2, -16), "../data/cylinder.obj"); // id 5
// }
//
// const std::vector<Collider>& Collision::getColliders() const {
//     return m_colliders;
// }
//
// glm::vec3 Collision::getMtv(int id) const {
//     if (id < 0 || id >= m_mtvs.size()) {
//         return glm::vec3(0.0f);
//     }
//     return m_mtvs[id];
// }
//
// CollisionMode Collision::getMode() const {
//     return m_mode;
// }
//
// void Collision::setMode(CollisionMode mType) {
//     m_mode = mType;
//
//     // Reset collider position and mtv
//     for (Collider& collider : m_colliders) {
//         collider.pos = collider.initPos;
//         m_mtvs[collider.id] = glm::vec3(0.f);
//     }
//
//     // Reset controlling collider
//     if (m_mode == Ellipsoid_Triangle) {
//         controllingCollider = 0;
//     } else if (m_mode == GJK_EPA) {
//         controllingCollider = 1;
//     }
// }
//
// void Collision::addCylinderCollider(glm::vec3 pos, float radius, float height) {
//     CylinderCollider c = CylinderCollider(index++, pos, radius, height);
//     m_cylinders.push_back(c);
//     m_colliders.push_back(c);
//     m_mtvs.push_back(glm::vec3(0.f));
// }
//
// void Collision::addEllipsoidCollider(glm::vec3 pos, glm::vec3 radii) {
//     EllipsoidCollider e = EllipsoidCollider(index++, pos, radii);
//     m_ellipsoids.push_back(e);
//     m_colliders.push_back(e);
//     m_mtvs.push_back(glm::vec3(0.f));
// }
//
// void Collision::addGJKCollider(glm::vec3 pos, const std::string& filepath) {
//     ConvexCollider g = ConvexCollider(index++, pos, filepath);
//     m_convexes.push_back(g);
//     m_colliders.push_back(g);
// }
//
// void Collision::handleKeyboardInput(const bool keys[]) {
//     if (keys[GLFW_KEY_UP]) m_colliders[controllingCollider].pos.z += 0.25;
//     if (keys[GLFW_KEY_DOWN]) m_colliders[controllingCollider].pos.z -= 0.25;
//     if (keys[GLFW_KEY_LEFT]) m_colliders[controllingCollider].pos.x += 0.25;
//     if (keys[GLFW_KEY_RIGHT]) m_colliders[controllingCollider].pos.x -= 0.25;
// }
//
// void Collision::changeControllingCollider() {
//     controllingCollider += 2;
//     controllingCollider %= (int) m_colliders.size();
// }
//
// // TODO: Implement for Ellipsoid-triangle
// void Collision::updateCylinderCylinderCollision() {
//     for (CylinderCollider cylinder : m_cylinders) {
//
//     }
// }
//
// // TODO: Implement for Ellipsoid-triangle
// void Collision::updateEllipsoidTriangleCollision() {
//     for (EllipsoidCollider ellipsoid : m_ellipsoids) {
//
//     }
// }
//
// // TODO: Implement for GJK-EPA
// void Collision::updateGJKCollision() {
//     for (ConvexCollider convex : m_convexes) {
//
//     }
// }