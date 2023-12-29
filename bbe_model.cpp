#include "bbe_model.hpp"
#include <cassert>
#include <cstring>
namespace bbe {
    BbeModel::BbeModel(BbeDevice &device, const std::vector<Vertex> &vertices) : bbeDevice{device} {
        createVertexBuffers(vertices);
    }
    BbeModel::~BbeModel() {
        vkDestroyBuffer(bbeDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(bbeDevice.device(), vertexBufferMemory, nullptr);
    }
    void BbeModel::createVertexBuffers(const std::vector<Vertex> &vertices) {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex Count must be at least 3");
        VkDeviceSize buffersize = sizeof(vertices[0]) * vertexCount;
        bbeDevice.createBuffer(
            buffersize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 

            // Allows Host to write to bit. CPU is host, GPU is device
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, //<-- keeps memory synced (hsot to device)
            vertexBuffer,
            vertexBufferMemory);
        void *data;
        // Creates a region of host memory and maps it to device memory (CPU to GPU)
        vkMapMemory(bbeDevice.device(), vertexBufferMemory, 0, buffersize, 0, &data);

        // We copy into host memory, becuase of coherency, it auto maps to device.
        memcpy(data, vertices.data(), static_cast<size_t>(buffersize));
        vkUnmapMemory(bbeDevice.device(), vertexBufferMemory);

    }
    void BbeModel::draw(VkCommandBuffer commandBuffer) {
        vkCmdDraw(commandBuffer, vertexCount,1, 0, 0);
    }
    void BbeModel::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer};
       VkDeviceSize offsets[] =  {0};
       vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    std::vector<VkVertexInputBindingDescription> BbeModel::Vertex::getBindingDescriptions(){
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }
    std::vector<VkVertexInputAttributeDescription> BbeModel::Vertex::getAttributeDescriptions() {
       std::vector<VkVertexInputAttributeDescription> attributeDeciptions(2);
       attributeDeciptions[0].binding = 0;
       attributeDeciptions[0].location = 0;
       attributeDeciptions[0].format = VK_FORMAT_R32G32_SFLOAT;
       attributeDeciptions[0].offset = offsetof(Vertex, position);

       attributeDeciptions[1].binding = 0;
       attributeDeciptions[1].location = 1; // THIS MATTERS
       attributeDeciptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
       attributeDeciptions[1].offset = offsetof(Vertex, color); // Calculates the Byte offset of the vertex struct 
       return attributeDeciptions;
    }
}