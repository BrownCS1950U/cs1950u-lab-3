//
// Created by Marcus Winter on 1/16/26.
//

#include "CollisionFunctions.h"

#include "Collider.h"
#include "../Debug.h"


MTV CollisionFunctions::sphereToSphere(const SphereCollider* sphere1, const SphereCollider* sphere2) {
    glm::vec3 pos1 = sphere1->transform->getPosition();
    glm::vec3 pos2 = sphere2->transform->getPosition();

    glm::vec3 delta = pos2 - pos1;
    float distance = glm::length(delta);
    float radiusSum = sphere1->radius + sphere2->radius;

    if (distance >= radiusSum) {
        return MTV{false};
    }

    float penetrationDepth = radiusSum - distance;
    glm::vec3 mtvDirection = -glm::normalize(delta);
    return {true, new glm::vec3(mtvDirection * penetrationDepth)};

}
