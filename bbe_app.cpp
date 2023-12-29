#include "bbe_app.hpp"
#include <stdexcept>
#include <iostream>
#include <array>

namespace bbe {

    FirstApp::FirstApp() {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }


    FirstApp::~FirstApp(){
        vkDestroyPipelineLayout(bbeDevice.device(), pipelineLayout, nullptr);
        // vkDestroyPipelineLayout(bbeDevice.device(), pipelineLayout, nullptr);
        // vkDestroyPipelineLayout(bbeDevice.device(), pipelineLayout, nullptr);
    }


    void FirstApp::run() {
        while (!bbeWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(bbeDevice.device());
        std::cout << "Done" << "\n";
    }


    void FirstApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(bbeDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout");
        }
    }


    void FirstApp::createPipeline() {
        auto pipelineConfig = BbePipeline::defaultPipelineConfigInfo(bbeSwapChain.width(), bbeSwapChain.height());
        pipelineConfig.renderPass = bbeSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        bbePipeline = std::make_unique<BbePipeline>(bbeDevice, "compiled_shaders/simple_shader.vert.spv", "compiled_shaders/simple_shader.frag.spv",
        pipelineConfig);

    }


    void FirstApp::createCommandBuffers() {
        commandBuffers.resize(bbeSwapChain.imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        // Priamry, can be executed, but not called by others.
        // Secondary, cannot be executed,  but canbe called by others.

        allocInfo.commandPool = bbeDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(bbeDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffers!");
        }

        //Need to record command buffers

        for (int i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("Fauled to begin recording command buffer");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = bbeSwapChain.getRenderPass();
            renderPassInfo.framebuffer = bbeSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0,0};
            renderPassInfo.renderArea.extent = bbeSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0}; // 0 = CLOSEST

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            // No mixing allowed, might not be useful
            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            bbePipeline->bind(commandBuffers[i]);

            // 3: 3 vertices, 1 Instance (Used for multiple copies of the same vertex data), 0 NO OFFSETS into our data... Even though we have no data.
            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to record command buffer");
            }
        }   

    }
    void FirstApp::drawFrame() {
        uint32_t imageIndex;
        auto result = bbeSwapChain.acquireNextImage(&imageIndex);
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image");
        }
        // We have submitted the image.
        result = bbeSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to present swap chain image!");
        }
    }
}