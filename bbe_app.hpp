#pragma once

#include "bbe_window.hpp"
#include "bbe_pipeline.hpp"
#include "bbe_device.hpp"
#include "bbe_swap_chain.hpp"
#include "bbe_model.hpp"
#include "bbe_engine_object.hpp"
#include <memory>
#include <vector>
namespace bbe {
    class FirstApp {
        public:

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &) = delete;
        FirstApp &operator=(const FirstApp &) = delete;


        static constexpr int WIDTH = 1920;
        static constexpr int HEIGHT = 1080;
        void run();
        private:

        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();
        void freeCommandBuffers();
        void sierpinski(std::vector<BbeModel::Vertex> &vertices,
            int depth,
            glm::vec2 left,
            glm::vec2 right,
            glm::vec2 top);
        void loadGameObjects();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        void renderGameObjects(VkCommandBuffer commandBuffer);

        


        BbeWindow bbeWindow{WIDTH, HEIGHT, "Blackbox"};
        BbeDevice bbeDevice{bbeWindow};
        // Order is important! ru like a stack.
        std::unique_ptr<BbeSwapChain> bbeSwapChain;
        // This is a smart pointer. Automatic Memory management
        std::unique_ptr<BbePipeline> bbePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<BbeGameObject> gameObjects;
        //  bbePipeline{bbeDevice, "./compiled_shaders/simple_shader.vert.spv", "./compiled_shaders/simple_shader.frag.spv", BbePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
        
    };
} 