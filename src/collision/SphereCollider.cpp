#include "SphereCollider.h"

#include "../GJK.h"

SphereCollider::SphereCollider(const std::shared_ptr<Transform>& transform):
    Collider(ColliderType::SPHERE, transform) {
}

MTV SphereCollider::getMTV(const Collider* other) const {
    return other->collideSphere(this);
}

glm::vec3 SphereCollider::getSupportPoint(const glm::vec3& direction) const {
    return GJK::getSphereSupportPoint(this, direction);
}

MTV SphereCollider::collideCylinder(const CylinderCollider* cylinder) const {
    return sphereCylinder(this, cylinder);

}

MTV SphereCollider::collideSphere(const SphereCollider* sphere) const {
    return sphereSphere(this, sphere);
}

MTV SphereCollider::collideBox(const BoxCollider* box) const {
    return sphereBox(this, box);
}



