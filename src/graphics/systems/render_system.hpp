#pragma once

#include "../hermes_camera.hpp"
#include "../hermes_pipeline.hpp"
#include "../hermes_device.hpp"
#include "../hermes_frame_info.hpp"

#include "../../game/hermes_game_object.hpp"

#include <memory>
#include <vector>

namespace HermesEngine
{
    class RenderSystem
    {
    public:
        RenderSystem(HermesEngineDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~RenderSystem();

        RenderSystem(const RenderSystem &) = delete;
        RenderSystem &operator=(const RenderSystem &) = delete;

        void renderGameObjects(FrameInfo &frameInfo);

    private:
        HermesEngineDevice &hermesDevice;

        std::unique_ptr<HermesPipeline> hermesPipeline;
        VkPipelineLayout pipelineLayout;

        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);
    };
}