#pragma once

#include "../render/Transform.h"
class SphereCollider;
class CylinderCollider;
class BoxCollider;
class ConvexCollider;
class TriangleMeshCollider;

enum ColliderType {
    BOX,
    CYLINDER,
    SPHERE,
    CONVEXHULL,
    TRIANGLE
};

struct MTV {
    bool collision = false;
    glm::vec3 normal = glm::vec3(0);
    float depth = 0.0f;

    MTV invert() const {
        return MTV{collision, -normal, depth};
    }
};

class Collider {
public:
    Collider(ColliderType type, const std::shared_ptr<Transform>& transform);
    virtual ~Collider() = default;
    virtual MTV getMTV(const Collider* other) const = 0;
    virtual glm::vec3 getSupportPoint(const glm::vec3& direction) const = 0;

    virtual MTV collideCylinder(const CylinderCollider* cylinder) const;
    virtual MTV collideSphere(const SphereCollider* sphere) const;
    virtual MTV collideBox(const BoxCollider* box) const;

    const ColliderType type;
    std::shared_ptr<Transform> transform;

    static MTV cylinderCylinder(const CylinderCollider* a, const CylinderCollider* b);
    static MTV cylinderBox(const CylinderCollider* a, const BoxCollider* b);
    static MTV cylinderSphere(const CylinderCollider* a, const SphereCollider* b);

    static MTV boxBox(const BoxCollider* a, const BoxCollider* b);
    static MTV boxSphere(const BoxCollider* a, const SphereCollider* b);

    static MTV sphereSphere(const SphereCollider* a, const SphereCollider* b);

    static MTV boxCylinder(const BoxCollider* a, const CylinderCollider* b);
    static MTV sphereCylinder(const SphereCollider* a, const CylinderCollider* b);
    static MTV sphereBox(const SphereCollider* a, const BoxCollider* b);
};


