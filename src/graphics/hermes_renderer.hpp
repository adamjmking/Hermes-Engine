#pragma once

#include "hermes_window.hpp"
#include "hermes_device.hpp"
#include "hermes_swap_chain.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace HermesEngine
{
    class HermesRenderer
    {
    public:
        HermesRenderer(HermesWindow &window, HermesEngineDevice &device);
        ~HermesRenderer();

        HermesRenderer(const HermesRenderer &) = delete;
        HermesRenderer &operator=(const HermesRenderer &) = delete;

        VkRenderPass getSwapChainRenderPass() const { return hermesSwapChain->getRenderPass(); }
        float getAspectRatio() const { return hermesSwapChain->extentAspectRatio(); }
        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const
        {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress.");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const
        {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress.");
            return currentFrameIndex;
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        HermesWindow &hermesWindow;
        HermesEngineDevice &hermesDevice;
        std::unique_ptr<HermesEngineSwapChain> hermesSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted{false};

        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();
    };
}