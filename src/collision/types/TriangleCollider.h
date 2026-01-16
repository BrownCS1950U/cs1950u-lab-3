#pragma once
#include "../Collider.h"


class TriangleCollider : public Collider {
public:
    TriangleCollider(ColliderType type, const std::shared_ptr<Transform>& transform)
        : Collider(type, transform) {
    }

    MTV getMTV(const Collider* other) const override;
    glm::vec3 getSupportPoint(const glm::vec3& direction) const override;
};
