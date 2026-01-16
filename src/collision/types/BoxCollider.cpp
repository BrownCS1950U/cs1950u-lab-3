#include "BoxCollider.h"

#include "../GJK.h"

BoxCollider::BoxCollider(const std::shared_ptr<Transform>& transform): Collider(ColliderType::BOX, transform) {

}

MTV BoxCollider::getMTV(const Collider* other) const {
    return other->collideBox(this);
}

glm::vec3 BoxCollider::getSupportPoint(const glm::vec3& direction) const {
    return GJK::getBoxSupportPoint(this, direction);
}

MTV BoxCollider::collideCylinder(const CylinderCollider* cylinder) const {
    return boxCylinder(this, cylinder);
}

MTV BoxCollider::collideSphere(const SphereCollider* sphere) const {
    return boxSphere(this, sphere);
}

MTV BoxCollider::collideBox(const BoxCollider* box) const {
    return boxBox(this, box);
}



