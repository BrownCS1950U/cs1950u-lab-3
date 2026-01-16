#pragma once

class TriangleCollider;
class ConvexCollider;
class SphereCollider;
class CylinderCollider;
class BoxCollider;
struct MTV;
class Collider;

struct Simplex {
    // TODO - use for constructing polytope with EPA
    // Fill this out with whatever you need. e.g array<glm::vec3, 4> points;
};
class GJK {

public:
    static bool detectCollision(const Collider* a, const Collider* b, Simplex& simplex);
    static MTV getMTV(const Collider* a, const Collider* b);

    // Support point functions for different collider types
    static glm::vec3 getBoxSupportPoint(const BoxCollider* box, const glm::vec3& direction);
    static glm::vec3 getCylinderSupportPoint(const CylinderCollider* cylinder, const glm::vec3& direction);
    static glm::vec3 getSphereSupportPoint(const SphereCollider* sphere, const glm::vec3& direction);
    static glm::vec3 getConvexSupportPoint(const ConvexCollider* convex, const glm::vec3& direction);
    static glm::vec3 getTriangleSupportPoint(const TriangleCollider* triangle, const glm::vec3& direction);
};
