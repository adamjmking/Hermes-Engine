#include "physics_system.hpp"

#include <iostream>

namespace HermesEngine
{
    HermesPhysicsSystem::HermesPhysicsSystem(const AABB &worldBoundary) : worldBoundary{worldBoundary} {}
    HermesPhysicsSystem::~HermesPhysicsSystem() {}

    void handleRigidbodyCollisions(HermesGameObject &obj1, HermesGameObject &obj2)
    {
        glm::vec3 pos1 = obj1.rigidbody->position;
        glm::vec3 pos2 = obj2.rigidbody->position;
        glm::vec3 vel1 = obj1.rigidbody->velocity;
        glm::vec3 vel2 = obj2.rigidbody->velocity;
        float r1 = obj1.rigidbody->radius;
        float r2 = obj2.rigidbody->radius;

        glm::vec3 diff = pos2 - pos1;
        float dist = glm::length(diff);
        float radiusSum = r1 + r2;

        if (dist < radiusSum)
        {
            glm::vec3 collisionNormal = glm::normalize(diff);
            float penetration = radiusSum - dist;

            glm::vec3 correction = collisionNormal * (penetration / 2.0f);

            obj1.rigidbody->position -= correction;
            obj2.rigidbody->position += correction;

            float restitution = 0.8f;
            glm::vec3 relativeVelocity = vel2 - vel1;
            float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);

            float j = -(1.0f + restitution) * velocityAlongNormal;
            j /= 2.0f;

            glm::vec3 impulse = j * collisionNormal;
            obj1.rigidbody->velocity -= correction;
            obj2.rigidbody->velocity += correction;
        }
    }

    void handleWorldBoundaryCollisions(HermesGameObject &obj, AABB worldBoundary)
    {
        glm::vec3 position = obj.rigidbody->position;
        glm::vec3 velocity = obj.rigidbody->velocity;
        float radius = obj.rigidbody->radius;

        // Check collisions with each boundary and resolve
        if (position.x - radius < worldBoundary.left())
        {
            position.x = worldBoundary.left() + radius;
            velocity.x = -velocity.x; // Reflect the velocity
        }
        else if (position.x + radius > worldBoundary.right())
        {
            position.x = worldBoundary.right() - radius;
            velocity.x = -velocity.x; // Reflect the velocity
        }

        if (position.y - radius < worldBoundary.top())
        {
            position.y = worldBoundary.top() + radius;
            velocity.y = -velocity.y; // Reflect the velocity
        }
        else if (position.y + radius > worldBoundary.bottom())
        {
            position.y = worldBoundary.bottom() - radius;
            velocity.y = -velocity.y; // Reflect the velocity
        }

        if (position.z - radius < worldBoundary.near())
        {
            position.z = worldBoundary.near() + radius;
            velocity.z = -velocity.z; // Reflect the velocity
        }
        else if (position.z + radius > worldBoundary.far())
        {
            position.z = worldBoundary.far() - radius;
            velocity.z = -velocity.z; // Reflect the velocity
        }

        // Update the Rigidbody's position and velocity
        obj.rigidbody->position = position;
        obj.rigidbody->velocity = velocity;
    }

    void HermesPhysicsSystem::updateRigidbodies(float dt, HermesGameObject::Map &gameObjects)
    {
        for (auto &kv : gameObjects)
        {
            auto &obj = kv.second;
            if (obj.rigidbody == nullptr)
                continue;

            obj.rigidbody->update(dt);
            obj.transform.translation = obj.rigidbody->position;
        }
    }

    void handleCollisions(float dt, HermesGameObject::Map &gameObjects, AABB worldBoundary)
    {
        for (auto &kv : gameObjects)
        {
            auto &obj = kv.second;
            if (obj.rigidbody == nullptr)
                continue;

            for (auto &kv2 : gameObjects)
            {
                auto &obj2 = kv.second;
                if (obj2.rigidbody == nullptr)
                    continue;
                if (obj.getId() == obj2.getId())
                    continue;

                handleRigidbodyCollisions(obj, obj2);
            }

            handleWorldBoundaryCollisions(obj, worldBoundary);
        }
    }

    void HermesPhysicsSystem::simulatePhysics(FrameInfo &frameInfo)
    {
        updateRigidbodies(frameInfo.frameTime, frameInfo.gameObjects);
        handleCollisions(frameInfo.frameTime, frameInfo.gameObjects, worldBoundary);
    }
}