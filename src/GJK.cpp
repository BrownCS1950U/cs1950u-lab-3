#include "GJK.h"
#include "Collider.h"
#include "collision/ConvexCollider.h"
#include "collision/TriangleCollider.h"


bool GJK::detectCollision(const Collider* a, const Collider* b, Simplex& simplex) {
    // TODO - implement GJK collision detection

    return false;
}

MTV GJK::getMTV(const Collider* a, const Collider* b) {
    // TODO - implement simplex (in GJK.h)
    Simplex simplex;
    if (detectCollision(a, b, simplex)) {
        // TODO - implement EPA to get MTV
    }
    return {};
}

// TODO - implement support point functions for different collider types
glm::vec3 GJK::getBoxSupportPoint(const BoxCollider* box, const glm::vec3& direction) {
    // TODO
    return {};
}

glm::vec3 GJK::getCylinderSupportPoint(const CylinderCollider* cylinder, const glm::vec3& direction) {
    // TODO
    return {};
}

glm::vec3 GJK::getSphereSupportPoint(const SphereCollider* sphere, const glm::vec3& direction) {
    // TODO
    return {};
}

glm::vec3 GJK::getConvexSupportPoint(const ConvexCollider* convex, const glm::vec3& direction) {
    // TODO
    auto points = convex->getPoints();
    return {};
}

glm::vec3 GJK::getTriangleSupportPoint(const TriangleCollider* triangle, const glm::vec3& direction) {
    // TODO
    auto vertices = triangle->getVertices();
    return {};
}
