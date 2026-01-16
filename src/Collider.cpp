#include "Collider.h"

#include "GJK.h"
#include "collision/BoxCollider.h"
#include "collision/CylinderCollider.h"
#include "collision/SphereCollider.h"


bool Collider::gjk_mode = false;


Collider::Collider(const ColliderType type, const std::shared_ptr<Transform>& transform):
    type(type), transform(transform) {

}

// Example
MTV Collider::sphereSphere(const SphereCollider* a, const SphereCollider* b) {
    const auto& scale_a = a->transform->scale_;
    const auto& scale_b = b->transform->scale_;

    const auto& pos_a = a->transform->getPosition();
    const auto& pos_b = b->transform->getPosition();

    // Can be improved to handle ellipsoids
    float radius_a = 0.5f * scale_a.x;
    float radius_b = 0.5f * scale_b.x;

    glm::vec3 direction = pos_b - pos_a;
    float distance = glm::length(direction);
    float radiusSum = radius_a + radius_b;

    if (distance >= radiusSum) return{false};

    float depth = radiusSum - distance;
    glm::vec3 normal = (distance > 0.0001f) ? direction / distance : glm::vec3(0,1,0);

    return {true, normal, depth};
}

MTV Collider::cylinderCylinder(const CylinderCollider* a, const CylinderCollider* b) {
    // TODO
    return {};
}

MTV Collider::cylinderBox(const CylinderCollider* a, const BoxCollider* b) {
    // TODO
    return {};
}

MTV Collider::cylinderSphere(const CylinderCollider* a, const SphereCollider* b) {
    // TODO
    return {};
}

MTV Collider::boxBox(const BoxCollider* a, const BoxCollider* b) {
    // TODO
    return {};
}

MTV Collider::boxSphere(const BoxCollider* a, const SphereCollider* b) {
    // TODO
    return {};
}


MTV Collider::boxCylinder(const BoxCollider* a, const CylinderCollider* b) {
    return cylinderBox(b, a).invert();
}

MTV Collider::sphereCylinder(const SphereCollider* a, const CylinderCollider* b) {

    return cylinderSphere(b, a).invert();
}

MTV Collider::sphereBox(const SphereCollider* a, const BoxCollider* b) {
    return boxSphere(b, a).invert();
}


// Defaults colliders to use GJK for collision detection if no specific implementation exists
MTV Collider::collideCylinder(const CylinderCollider* cylinder) const {
    return GJK::getMTV(this, cylinder);
}

MTV Collider::collideSphere(const SphereCollider* sphere) const {
    return GJK::getMTV(this, sphere);
}

MTV Collider::collideBox(const BoxCollider* box) const {
    return GJK::getMTV(this, box);
}

