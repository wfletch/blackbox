#include "bbe_app.hpp"
#include <stdexcept>
#include <iostream>
#include <array>
#include <ctime>
#include <cstdlib> 

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE //0 to -1, not -1 to 1
#include "glm/glm.hpp"


namespace bbe {
    struct SimplePushConstantData {
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };
    FirstApp::FirstApp() {
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
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
    
    void FirstApp::sierpinski(
        std::vector<BbeModel::Vertex> &vertices,
        int depth,
        glm::vec2 left,
        glm::vec2 right,
        glm::vec2 top) {
    if (depth <= 0) {
        vertices.push_back({top, {1.0f,0.0f,0.0f}});
        vertices.push_back({right, {0.0f,1.0f,0.0f}});
        vertices.push_back({left, {0.0f,0.0f,1.0f}});
    } else {
        auto leftTop = 0.5f * (left + top);
        auto rightTop = 0.5f * (right + top);
        auto leftRight = 0.5f * (left + right);
        sierpinski(vertices, depth - 1, left, leftRight, leftTop);
        sierpinski(vertices, depth - 1, leftRight, right, rightTop);
        sierpinski(vertices, depth - 1, leftTop, rightTop, top);
    }
    }
    void FirstApp::loadModels() {
        std::vector<BbeModel::Vertex> vertices{};
        srand((unsigned int)time(NULL));
        for (int x = 0; x < 50; x++) {
            for (int y =0; y < 50; y++)
            {
                float rand_r = (rand()%10) / 10.0;
                float rand_g = (rand()%10) / 10.0;
                float rand_b = (rand()%10) / 10.0;
                float x_loc = -1.0f + (((rand()%11)/10.0) * 2);
                float y_loc = -1.0f + (((rand()%11)/10.0) * 2); 
                // std::cout << y_loc << "\n";
            vertices.push_back({{x_loc,y_loc}, {rand_r,rand_g,rand_b}});
            }
        }
        bbeModel = std::make_unique<BbeModel>(bbeDevice, vertices);
    }

    void FirstApp::createPipelineLayout() {


        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof (SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(bbeDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout");
        }
    }


    void FirstApp::createPipeline() {
        assert(bbeSwapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
        PipelineConfigInfo pipelineConfig{};
        BbePipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = bbeSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        bbePipeline = std::make_unique<BbePipeline>(bbeDevice, "compiled_shaders/simple_shader.vert.spv", "compiled_shaders/simple_shader.frag.spv",
        pipelineConfig);

    }


    void FirstApp::createCommandBuffers() {
        commandBuffers.resize(bbeSwapChain->imageCount());
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

        

    }
    void FirstApp::recordCommandBuffer(int imageIndex){
            static int frame = 0;
            frame = (frame + 1) % 1000;
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("Fauled to begin recording command buffer");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = bbeSwapChain->getRenderPass();
            renderPassInfo.framebuffer = bbeSwapChain->getFrameBuffer(imageIndex);

            renderPassInfo.renderArea.offset = {0,0};
            renderPassInfo.renderArea.extent = bbeSwapChain->getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.01f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0}; // 0 = CLOSEST

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            // No mixing allowed, might not be useful
            vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(bbeSwapChain->getSwapChainExtent().width);
            viewport.height = static_cast<float>(bbeSwapChain->getSwapChainExtent().height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            VkRect2D scissor {{0,0}, bbeSwapChain->getSwapChainExtent()};
            vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
            vkCmdSetScissor(commandBuffers[imageIndex], 0,1, &scissor);
             
            bbePipeline->bind(commandBuffers[imageIndex]);
            bbeModel->bind(commandBuffers[imageIndex]);
            
            for (int j = 0; j< 1; j++) {
                SimplePushConstantData push {};
                float offset_x = (((rand()%11)/10.0)) ;
                float offset_y = (((rand()%11)/10.0));
                // float offset_z = (((rand()%11)/10.0));
                push.offset = {offset_x + frame * 0.002f ,offset_y + frame* 0.002f}; 

                vkCmdPushConstants(commandBuffers[imageIndex], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0 ,sizeof(SimplePushConstantData), &push);
                bbeModel->draw(commandBuffers[imageIndex]);
                
            }
        
            
            vkCmdEndRenderPass(commandBuffers[imageIndex]);
            if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to record command buffer");
            }
           
    }
    void FirstApp::freeCommandBuffers(){
        vkFreeCommandBuffers(bbeDevice.device(),bbeDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }
    
    void FirstApp::recreateSwapChain() {
        auto extent = bbeWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = bbeWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(bbeDevice.device());

        if (bbeSwapChain == nullptr) {
            bbeSwapChain = std::make_unique<BbeSwapChain>(bbeDevice, extent);
            createPipeline();
        } else {
            bbeSwapChain = std::make_unique<BbeSwapChain>(bbeDevice, extent, std::move(bbeSwapChain));
            if (bbeSwapChain->imageCount() != commandBuffers.size()){
                freeCommandBuffers();
                createCommandBuffers();
            }
        }
    }
    void FirstApp::drawFrame() {
        uint32_t imageIndex;
        auto result = bbeSwapChain->acquireNextImage(&imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to acquire swap chain image");
        }
        // We have submitted the image.
        recordCommandBuffer(imageIndex);
        result = bbeSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || bbeWindow.wasWindowResized()){
          bbeWindow.resetWindowResizedFlag();
          recreateSwapChain();
          return;  
         }
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to present swap chain image!");
        }
    }
}