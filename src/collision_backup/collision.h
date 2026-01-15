// #ifndef VIEWER_COLLISION_H
// #define VIEWER_COLLISION_H
//
// #include <vector>
// #include <glm/glm.hpp>
// #include "collider.h"
//
// enum CollisionMode {
//     Ellipsoid_Triangle,
//     GJK_EPA
// };
//
// class Collision {
// public:
//     Collision();
//     void updateCylinderCylinderCollision();
//     void updateEllipsoidTriangleCollision();
//     void updateGJKCollision();
//     void handleKeyboardInput(const bool keys[]);
//
//     const std::vector<Collider> &getColliders() const;
//     glm::vec3 getMtv(int id) const;
//     CollisionMode getMode() const;
//
//     void setMode(CollisionMode mType);
//     void changeControllingCollider();
//
// private:
//     CollisionMode m_mode = Ellipsoid_Triangle; // Default to Ellipsoid-Triangle mode
//     StaticMesh m_mesh;
//     std::vector<CylinderCollider> m_cylinders;
//     std::vector<EllipsoidCollider> m_ellipsoids;
//     std::vector<ConvexCollider> m_convexes;
//
//     int index = 0;
//     int controllingCollider = 0;
//     std::vector<Collider> m_colliders;
//     std::vector<glm::vec3> m_mtvs;
//
//     void addCylinderCollider(glm::vec3 pos, float radius, float height);
//     void addEllipsoidCollider(glm::vec3 pos, glm::vec3 radii);
//     void addGJKCollider(glm::vec3 pos, const std::string& filepath);
// };
//
//
// #endif //VIEWER_COLLISION_H
