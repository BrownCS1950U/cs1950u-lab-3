#pragma once
#include "../Collider.h"


class ConeCollider : public Collider{
public:
    ConeCollider(const std::shared_ptr<Transform>& transform);
    MTV getMTV(const Collider* other) const override;
    glm::vec3 getSupportPoint(const glm::vec3& direction) const override;
};
