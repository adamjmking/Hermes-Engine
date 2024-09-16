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
    class PointLightRenderSystem
    {
    public:
        PointLightRenderSystem(HermesEngineDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~PointLightRenderSystem();

        PointLightRenderSystem(const PointLightRenderSystem &) = delete;
        PointLightRenderSystem &operator=(const PointLightRenderSystem &) = delete;

        void update(FrameInfo &frameInfo, GlobalUbo &ubo);
        void render(FrameInfo &frameInfo);

    private:
        HermesEngineDevice &hermesDevice;

        std::unique_ptr<HermesPipeline> hermesPipeline;
        VkPipelineLayout pipelineLayout;

        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);
    };
}