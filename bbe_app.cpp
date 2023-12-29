#include "bbe_app.hpp"
#include <stdexcept>

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
        }
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

    
    void FirstApp::createCommandBuffers() {}
    void FirstApp::drawFrame() {}
}