#include "app.hpp"

#include "graphics/hermes_camera.hpp"
#include "graphics/hermes_buffer.hpp"
#include "graphics/systems/render_system.hpp"
#include "graphics/systems/point_light_system.hpp"

#include "game/keyboard_movement_controller.hpp"
#include "game/mouse_spawn_physics_object.hpp"

#include "physics/physics_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <stdexcept>
#include <array>
#include <chrono>

namespace HermesEngine
{
    App::App()
    {
        globalPool = HermesDescriptorPool::Builder(hermesDevice)
                         .setMaxSets(HermesEngineSwapChain::MAX_FRAMES_IN_FLIGHT)
                         .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, HermesEngineSwapChain::MAX_FRAMES_IN_FLIGHT)
                         .build();
        loadGameObjects();
    }

    App::~App() {}

    void App::run()
    {
        std::vector<std::unique_ptr<HermesBuffer>> uboBuffers(HermesEngineSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++)
        {
            uboBuffers[i] = std::make_unique<HermesBuffer>(
                hermesDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout = HermesDescriptorSetLayout::Builder(hermesDevice)
                                   .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
                                   .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(HermesEngineSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            HermesDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        RenderSystem renderSystem{hermesDevice, hermesRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        PointLightRenderSystem pointLightSystem{hermesDevice, hermesRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        HermesPhysicsSystem physicsSystem{AABB{glm::vec3{-5.f, 0.5f, -5.f}, glm::vec3{5.f, -20.f, 5.f}}};

        HermesCamera camera{};

        KeyboardMovementController cameraController{};
        MouseSpawnPhysicsObject physicsObjectSpawner{};
        physicsObjectSpawner.init(hermesDevice);

        auto viewerObject = HermesGameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;

        auto currentTime = std::chrono::high_resolution_clock::now();
        while (!hermesWindow.shouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;
            frameTime = glm::min(frameTime, App::MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(hermesWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            physicsObjectSpawner.trySpawnPhysicsObject(hermesWindow.getGLFWwindow(), gameObjects);

            float aspectRatio = hermesRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspectRatio, 0.1f, 100.f);

            if (auto commandBuffer = hermesRenderer.beginFrame())
            {
                int frameIndex = hermesRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                    gameObjects};

                // physics simulation
                physicsSystem.simulatePhysics(frameInfo);

                // update views
                GlobalUbo ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                ubo.inverseView = camera.getInverseView();
                pointLightSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);

                // render
                hermesRenderer.beginSwapChainRenderPass(commandBuffer);

                renderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);

                // output frame data
                std::cout << "FPS: " << 1 / frameTime << std::endl;
                std::cout << "Number of objects: " << gameObjects.size() << std::endl;

                hermesRenderer.endSwapChainRenderPass(commandBuffer);
                hermesRenderer.endFrame();
            }
        }
        vkDeviceWaitIdle(hermesDevice.device());
    }

    void loadGroundQuad(HermesEngineDevice &hermesDevice, HermesGameObject::Map &gameObjects)
    {
        std::shared_ptr<HermesModel> quadModel = HermesModel::createModelFromFile(hermesDevice, "assets/quad.obj");
        auto quad = HermesGameObject::createGameObject();
        quad.model = quadModel;
        quad.transform.translation = {0.f, 0.5f, 0.f};
        quad.transform.scale = glm::vec3{5.f};
        gameObjects.emplace(quad.getId(), std::move(quad));
    }

    void loadPointLights(HermesGameObject::Map &gameObjects)
    {
        std::vector<glm::vec3> colors{
            {1.f, .1f, .1f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f}};

        for (int i = 0; i < 6; i++)
        {
            auto light = HermesGameObject::createPointLight(0.5f);
            light.color = colors[i];

            auto rotateLight = glm::rotate(
                glm::mat4(1.f),
                (i * glm::two_pi<float>()) / 6,
                {0.f, -1.f, 0.f});
            light.transform.translation = glm::vec3(rotateLight * glm::vec4(-2.f, -1.f, -2.f, 2.f));

            gameObjects.emplace(light.getId(), std::move(light));
        }
    }

    void App::loadGameObjects()
    {
        loadGroundQuad(hermesDevice, gameObjects);
        loadPointLights(gameObjects);

        std::shared_ptr<HermesModel> smoothVaseModel = HermesModel::createModelFromFile(hermesDevice, "assets/sphere.obj");
        auto smoothVase = HermesGameObject::createGameObject();
        smoothVase.model = smoothVaseModel;
        smoothVase.transform.translation = {-1.f, 0.f, 0.f};
        smoothVase.transform.scale = glm::vec3{0.1f};
        gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

        auto smoothVase2 = HermesGameObject::createGameObject();
        smoothVase2.model = smoothVaseModel;
        smoothVase2.transform.translation = {1.f, 0.f, 0.f};
        smoothVase2.transform.scale = glm::vec3{0.1f};
        gameObjects.emplace(smoothVase2.getId(), std::move(smoothVase2));
    }
}