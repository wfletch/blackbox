#include "bbe_app.hpp"
#include <stdexcept>
#include <iostream>
#include <array>
#include <ctime>
#include <cstdlib> 

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE //0 to -1, not -1 to 1
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"


namespace bbe {
    struct SimplePushConstantData {
        glm::mat2 transform{1.f}; //main Diagonal
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };
    FirstApp::FirstApp() {
        loadGameObjects();
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
    void FirstApp::loadGameObjects() {
        std::vector<BbeModel::Vertex> vertices_back{};
        std::vector<BbeModel::Vertex> vertices_front{};

        float mult_factor = 65.0f;
        vertices_back.push_back({{-1.0f/mult_factor,-1.0f/mult_factor}, {0.0f,1.0f, 1.0f}});
        vertices_back.push_back({{1.0f/mult_factor,-1.0f/mult_factor}, {0.0f,1.0f, 1.0f}});
        vertices_back.push_back({{-1.0f/mult_factor,1.0f/mult_factor}, {0.0f,1.0f, 1.0f}});
        
        // mult_factor = 0.8f;
        // vertices_front.push_back({{-1.0f/mult_factor,-1.0f/mult_factor}, {0.0f,1.0f, 1.0f}});
        // vertices_front.push_back({{1.0f/mult_factor,-1.0f/mult_factor}, {0.0f,1.0f, 1.0f}});
        // vertices_front.push_back({{-1.0f/mult_factor,1.0f/mult_factor}, {0.0f,1.0f, 1.0f}});


        // //This is an Object
        // vertices.push_back({{-1.0f/2.0f,-1.0f/2.0f}, {0.0f,1.0f, 1.0f}});
        // vertices.push_back({{1.0f/2.0f,-1.0f/2.0f}, {0.0f,1.0f, 1.0f}});
        // vertices.push_back({{-1.0f/2.0f,1.0f/2.0f}, {0.0f,1.0f, 1.0f}});
        // vertices.push_back({{1.0f/2.0f,-1.0f/2.0f}, {0.0f,1.0f, 1.0f}});
        // vertices.push_back({{1.0f/2.0f,1.0f/2.0f}, {0.0f,1.0f, 1.0f}});
        // vertices.push_back({{-1.0f/2.0f,1.0f/2.0f}, {0.0f,1.0f, 1.0f}});


        // vertices.push_back({{-1.0f/2.0f,-1.0f/2.0f}, {0.0f,1.0f, 1.0f}});
        // vertices.push_back({{1.0f/2.0f,-1.0f/2.0f}, {0.0f,1.0f, 1.0f}});
        // vertices.push_back({{-1.0f/2.0f,1.0f/2.0f}, {0.0f,1.0f, 1.0f}});
        // vertices.push_back({{1.0f/2.0f,-1.0f/2.0f}, {0.0f,1.0f, 1.0f}});
        // vertices.push_back({{1.0f/2.0f,1.0f/2.0f}, {0.0f,1.0f, 1.0f}});
        // vertices.push_back({{-1.0f/2.0f,1.0f/2.0f}, {0.0f,1.0f, 1.0f}});

        // // Background
        // vertices.push_back({{-1.0f,-1.0f}, {1.0f,1.0f, 1.0f}});
        // vertices.push_back({{1.0f,-1.0f}, {1.0f,1.0f, 1.0f}});
        // vertices.push_back({{-1.0f,1.0f}, {1.0f,1.0f, 1.0f}});
        // vertices.push_back({{1.0f,-1.0f}, {1.0f,1.0f, 1.0f}});
        // vertices.push_back({{1.0f,1.0f}, {1.0f,1.0f, 1.0f}});
        // vertices.push_back({{-1.0f,1.0f}, {1.0f,1.0f, 1.0f}});

        std::vector<glm::vec3> colors{
        {79/255.0, 93/255.0, 117/255.0},
        {191/255.0, 192.0/255.0, 192.0/255.0},
         {255.0/255.0, 255.0/255.0, 255.0/255.0},
        {239.0/255.0, 131.0/255.0, 84.0/255.0},
        // {198.0/255.0, 226.0/255.0, 170.0/255.0}  //
        };       
    for (int i = 30; i > 0; i-=3) {
        auto bbeModel_back = std::make_shared<BbeModel>(bbeDevice, vertices_back);
        // auto bbeModel_front= std::make_shared<BbeModel>(bbeDevice, vertices_front);
        auto triangle = BbeGameObject::createGameObject();
        triangle.model = bbeModel_back;
        triangle.color = colors[i % colors.size()];
        if (i%4 == 0) {
            triangle.transform2d.translation.x = (i * 0.017);
            triangle.transform2d.translation.y = (-i * 0.03);
        }
        else if (i % 3 == 0) {
            triangle.transform2d.translation.y = (i * 0.02);
            triangle.transform2d.translation.x = (-i * 0.05);
            /* code */
        } else if (i % 2  == 0)
        {
            /* code */
            triangle.transform2d.translation.y = (-i * 0.01);
            triangle.transform2d.translation.x = (-i * 0.02);
        } else {

            triangle.transform2d.translation.y = (i * 0.02);
            triangle.transform2d.translation.x = (i * 0.023);
        }
        
        
        
        triangle.transform2d.scale = glm::vec2(.9f) + i * 0.23f;
        triangle.transform2d.rotation = i * glm::pi<float>() * .286f;

        // gameObjects.push_back(std::move(triangle));
        // triangle.model = bbeModel_front;
        // triangle.color = colors[(i+3) % colors.size()];
        // triangle.transform2d.translation.x = .01f;
        // triangle.transform2d.scale = glm::vec2(.5f) + i * 0.025f;
        // // triangle.transform2d.rotation = i * glm::pi<float>() * .025f;
        gameObjects.push_back(std::move(triangle));
        }
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
            clearValues[0].color = {25.0/255.0f, 19.0/255.0f, 26/255.0f, 0.8f};
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
             
            renderGameObjects(commandBuffers[imageIndex]);
            
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

    void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer) {

        int i = 0;
        for (auto& obj : gameObjects) {
            i += 1;
            obj.transform2d.rotation = glm::mod<float>(obj.transform2d.rotation + 0.0001f * i, 2.f * glm::pi<float>());
        }

        bbePipeline->bind(commandBuffer);
        for (auto& obj: gameObjects) {
            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.001f, glm::two_pi<float>());
            SimplePushConstantData push {};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();

            vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0 ,sizeof(SimplePushConstantData), &push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
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