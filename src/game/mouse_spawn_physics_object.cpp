#include "mouse_spawn_physics_object.hpp"

#include <iostream>

namespace HermesEngine
{
    void MouseSpawnPhysicsObject::trySpawnPhysicsObject(GLFWwindow *window, HermesGameObject::Map &gameObjects)
    {
        if (glfwGetMouseButton(window, keys.spawnPhysicsObject) == GLFW_PRESS)
        {
            for (int i = 0; i < 10; i++)
            {
                auto randPosition = glm::vec3{randBetween(-4.0f, 4.0f), 0.f, randBetween(-4.0f, 4.0f)};
                auto physicsBall = HermesGameObject::createRigidbody();

                physicsBall.model = sphereModel;
                physicsBall.transform.translation = randPosition;
                physicsBall.transform.scale = glm::vec3{0.1f};
                physicsBall.rigidbody->position = randPosition;

                gameObjects.emplace(physicsBall.getId(), std::move(physicsBall));
            }
        }
    }
}