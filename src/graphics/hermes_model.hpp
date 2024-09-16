#pragma once

#include "hermes_device.hpp"
#include "hermes_buffer.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace HermesEngine
{
    class HermesModel
    {
    public:
        struct Vertex
        {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex &other) const
            {
                return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
            }
        };

        struct Builder
        {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            void loadModel(const std::string &filepath);
        };

        HermesModel(HermesEngineDevice &device, const HermesModel::Builder &builder);
        ~HermesModel();

        HermesModel(const HermesModel &) = delete;
        void operator=(const HermesModel &) = delete;

        static std::unique_ptr<HermesModel> createModelFromFile(HermesEngineDevice &device, const std::string &filepath);

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        HermesEngineDevice &hermesDevice;

        std::unique_ptr<HermesBuffer> vertexBuffer;
        uint32_t vertexCount;

        std::unique_ptr<HermesBuffer> indexBuffer;
        bool hasIndexBuffer = false;
        uint32_t indexCount;

        void createVertexBuffers(const std::vector<Vertex> &vertices);
        void createIndexBuffers(const std::vector<uint32_t> &indices);
    };
}