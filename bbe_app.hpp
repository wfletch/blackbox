#pragma once

#include "bbe_window.hpp"
#include "bbe_pipeline.hpp"
#include "bbe_device.hpp"
#include "bbe_swap_chain.hpp"
#include "bbe_model.hpp"
#include <memory>
#include <vector>
namespace bbe {
    class FirstApp {
        public:

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &) = delete;
        FirstApp &operator=(const FirstApp &) = delete;


        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
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
        void loadModels();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);

        


        BbeWindow bbeWindow{WIDTH, HEIGHT, "Hello, Vulkan!"};
        BbeDevice bbeDevice{bbeWindow};
        // Order is important! ru like a stack.
        std::unique_ptr<BbeSwapChain> bbeSwapChain;
        // This is a smart pointer. Automatic Memory management
        std::unique_ptr<BbePipeline> bbePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<BbeModel> bbeModel;
        //  bbePipeline{bbeDevice, "./compiled_shaders/simple_shader.vert.spv", "./compiled_shaders/simple_shader.frag.spv", BbePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
        
    };
} 