#pragma once

#include "bbe_device.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE //0 to -1, not -1 to 1
#include "glm/glm.hpp"


#include <vector>
namespace bbe {
    class BbeModel {
        // Take vertex data from our cpu
        // allocate the memory and copy over to device gpu
    public:
    struct Vertex {
        glm::vec2 position;
        glm::vec3 color;
        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };
    BbeModel(BbeDevice &BbeDevice, const std::vector<Vertex> &vertices);
    ~BbeModel();
    BbeModel(const BbeModel &) = delete;
    BbeModel operator=(const BbeModel &) = delete;

    void bind(VkCommandBuffer VkCommandBuffer);
    void draw(VkCommandBuffer VkCommandBuffer);
    private:
    BbeDevice& bbeDevice;
    VkBuffer vertexBuffer;
    void createVertexBuffers(const std::vector<Vertex> &vertices);
    // Buffer and assigned memory are two different objects.
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;
    };
}