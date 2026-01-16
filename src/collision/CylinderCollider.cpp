#include "CylinderCollider.h"

#include "../GJK.h"

CylinderCollider::CylinderCollider(const std::shared_ptr<Transform>& transform):
    Collider(ColliderType::CYLINDER, transform) {

}

MTV CylinderCollider::getMTV(const Collider* other) const {
    return other->collideCylinder(this);
}

glm::vec3 CylinderCollider::getSupportPoint(const glm::vec3& direction) const {
    return GJK::getCylinderSupportPoint(this, direction);
}


MTV CylinderCollider::collideCylinder(const CylinderCollider* cylinder) const {
    return cylinderCylinder(this, cylinder);
}

MTV CylinderCollider::collideSphere(const SphereCollider* sphere) const {
    return cylinderSphere(this, sphere);
}

MTV CylinderCollider::collideBox(const BoxCollider* box) const {
    return cylinderBox(this, box);
}
