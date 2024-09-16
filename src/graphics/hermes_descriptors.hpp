#pragma once

#include "hermes_device.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace HermesEngine
{
    class HermesDescriptorSetLayout
    {
    public:
        class Builder
        {
        public:
            Builder(HermesEngineDevice &hermesDevice) : hermesDevice{hermesDevice} {}

            Builder &addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<HermesDescriptorSetLayout> build() const;

        private:
            HermesEngineDevice &hermesDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        HermesDescriptorSetLayout(
            HermesEngineDevice &hermesDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~HermesDescriptorSetLayout();
        HermesDescriptorSetLayout(const HermesDescriptorSetLayout &) = delete;
        HermesDescriptorSetLayout &operator=(const HermesDescriptorSetLayout &) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        HermesEngineDevice &hermesDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class HermesDescriptorWriter;
    };

    class HermesDescriptorPool
    {
    public:
        class Builder
        {
        public:
            Builder(HermesEngineDevice &hermesDevice) : hermesDevice{hermesDevice} {}

            Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder &setMaxSets(uint32_t count);
            std::unique_ptr<HermesDescriptorPool> build() const;

        private:
            HermesEngineDevice &hermesDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        HermesDescriptorPool(
            HermesEngineDevice &hermesDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize> &poolSizes);
        ~HermesDescriptorPool();
        HermesDescriptorPool(const HermesDescriptorPool &) = delete;
        HermesDescriptorPool &operator=(const HermesDescriptorPool &) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

        void resetPool();

    private:
        HermesEngineDevice &hermesDevice;
        VkDescriptorPool descriptorPool;

        friend class HermesDescriptorWriter;
    };

    class HermesDescriptorWriter
    {
    public:
        HermesDescriptorWriter(HermesDescriptorSetLayout &setLayout, HermesDescriptorPool &pool);

        HermesDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
        HermesDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

        bool build(VkDescriptorSet &set);
        void overwrite(VkDescriptorSet &set);

    private:
        HermesDescriptorSetLayout &setLayout;
        HermesDescriptorPool &pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}