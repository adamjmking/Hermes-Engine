#pragma once

#include "../graphics/hermes_frame_info.hpp"
#include "../game/hermes_game_object.hpp"

#include "AABB.hpp"

namespace HermesEngine
{
    class HermesPhysicsSystem
    {
    public:
        HermesPhysicsSystem(const AABB &worldBoundary);
        ~HermesPhysicsSystem();

        HermesPhysicsSystem(const HermesPhysicsSystem &) = delete;
        HermesPhysicsSystem &operator=(const HermesPhysicsSystem &) = delete;

        void simulatePhysics(FrameInfo &frameInfo);

    private:
        AABB worldBoundary;

        void updateRigidbodies(float dt, HermesGameObject::Map &gameObjects);
    };
}