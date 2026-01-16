#include "TriangleCollider.h"

#include "../GJK.h"

MTV TriangleCollider::getMTV(const Collider* other) const {
    return GJK::getMTV(this, other);
}

glm::vec3 TriangleCollider::getSupportPoint(const glm::vec3& direction) const {
    return GJK::getTriangleSupportPoint(this, direction);
}
