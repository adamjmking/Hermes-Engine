#pragma once

#include "hermes_game_object.hpp"
#include "../graphics/hermes_model.hpp"
#include "../graphics/hermes_window.hpp"
#include "../graphics/hermes_device.hpp"

namespace HermesEngine
{
    class MouseSpawnPhysicsObject
    {
    public:
        struct KeyMappings
        {
            int spawnPhysicsObject = GLFW_MOUSE_BUTTON_1;
        };
        KeyMappings keys{};

        void trySpawnPhysicsObject(GLFWwindow *window, HermesGameObject::Map &gameObjects);
        void init(HermesEngineDevice &device)
        {
            sphereModel = HermesModel::createModelFromFile(device, "assets/sphere.obj");
        }

    private:
        std::shared_ptr<HermesModel> sphereModel;

        float randBetween(float low, float high)
        {
            return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (high - low)));
        }
    };
}
