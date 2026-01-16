#include "ConvexCollider.h"

#include "../GJK.h"

ConvexCollider::ConvexCollider(const std::shared_ptr<Transform>& transform, const std::vector<glm::vec3>& points) :
    Collider(ColliderType::CONVEXHULL, transform), points_(points) {
}

MTV ConvexCollider::getMTV(const Collider* other) const {
    return GJK::getMTV(this, other);
}

glm::vec3 ConvexCollider::getSupportPoint(const glm::vec3& direction) const {
    return GJK::getConvexSupportPoint(this, direction);
}
