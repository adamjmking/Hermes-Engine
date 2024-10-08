#pragma once

#include "hermes_device.hpp"

#include <string>
#include <vector>

namespace HermesEngine
{
    struct PipelineConfigInfo
    {
        PipelineConfigInfo() = default;
        PipelineConfigInfo(const PipelineConfigInfo &) = delete;
        PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

        std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class HermesPipeline
    {
    public:
        HermesPipeline(HermesEngineDevice &device, const std::string &vertShaderPath, const std::string &fragShaderPath, const PipelineConfigInfo &configInfo);
        ~HermesPipeline();

        HermesPipeline(const HermesPipeline &) = delete;
        HermesPipeline &operator=(const HermesPipeline &) = delete;

        void bind(VkCommandBuffer commandBuffer);
        static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);
        static void enableAlphaBlending(PipelineConfigInfo &configInfo);

    private:
        static std::vector<char> readFile(const std::string &filepath);
        void createGraphicsPipeline(const std::string &vertShaderPath, const std::string &fragShaderPath, const PipelineConfigInfo &configInfo);

        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        HermesEngineDevice &hermesDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}