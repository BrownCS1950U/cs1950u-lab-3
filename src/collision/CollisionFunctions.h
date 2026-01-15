#pragma once


struct MTV;
class SphereCollider;

class CollisionFunctions {

public:

    static MTV sphereToSphere(const SphereCollider* sphere1, const SphereCollider* sphere2);
    // static glm::vec3 boxToBox(const BoxCollider* box1, const BoxCollider* box2);
};
