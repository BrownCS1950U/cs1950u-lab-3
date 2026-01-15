#pragma once

#include <utility>

#include "CollisionFunctions.h"
#include "../render/Transform.h"
enum class ColliderType {
    BOX,
    CYLINDER,
    SPHERE,
    CONVEXHULL
};

struct MTV {
    bool collision;
    glm::vec3* mtv;
};


class Collider {
public:
    Collider(const ColliderType type, const std::shared_ptr<Transform>& transform) :
        type(type), transform(transform) {}
    virtual ~Collider() = default;
    virtual MTV getMTV(const Collider& other) const = 0;

    virtual glm::vec3 getSupportPoint(const glm::vec3& direction) const {;
        return glm::vec3(0.0f); // Implement for GJK
    }



    const ColliderType type;
    std::shared_ptr<Transform> transform;
};


class SphereCollider : public Collider {
public:
    SphereCollider(const std::shared_ptr<Transform>& transform) :
        Collider(ColliderType::SPHERE, transform) {
        assert(transform->getScale().x == transform->getScale().y &&
               transform->getScale().y == transform->getScale().z &&
               "SphereCollider requires uniform scale");
        radius = 0.5f * transform->getScale().x;
    }

    MTV getMTV(const Collider& other) const override {
        const auto* otherSphere = dynamic_cast<const SphereCollider*>(&other);
        return CollisionFunctions::sphereToSphere(this, otherSphere);
    }

    float radius;
};