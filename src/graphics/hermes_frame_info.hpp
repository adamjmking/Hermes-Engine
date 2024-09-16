#pragma once

#include "hermes_camera.hpp"
#include "../game/hermes_game_object.hpp"

#include <vulkan/vulkan.h>

namespace HermesEngine
{
#define MAX_LIGHTS 10

    struct PointLight
    {
        glm::vec4 position{}; // ignore w
        glm::vec4 color{};    // w is intensity
    };

    struct GlobalUbo
    {
        glm::mat4 projection{1.f};
        glm::mat4 view{1.f};
        glm::mat4 inverseView{1.f};
        glm::vec4 ambientLightColor{1.f, 1.f, 1.f, 0.02f}; // w is light intensity
        PointLight pointLights[MAX_LIGHTS];
        int numLights;
    };

    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        HermesCamera &camera;
        VkDescriptorSet globalDescriptorSet;
        HermesGameObject::Map &gameObjects;
    };

}