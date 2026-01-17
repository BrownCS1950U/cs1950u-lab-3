#include "ConeCollider.h"

#include "../GJK.h"

ConeCollider::ConeCollider(const std::shared_ptr<Transform>& transform): Collider(CONE, transform) {
}

MTV ConeCollider::getMTV(const Collider* other) const {
    return GJK::getMTV(this, other);
}

glm::vec3 ConeCollider::getSupportPoint(const glm::vec3& direction) const {
    return GJK::getConeSupportPoint(this, direction);
}
