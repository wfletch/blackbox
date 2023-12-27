#pragma once

#include "bbe_window.hpp"
#include "bbe_pipeline.hpp"
namespace bbe {
    class FirstApp {
        public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        void run();
        private:
        BbeWindow bbeWindow{WIDTH, HEIGHT, "Hello, Vulkan!"};
        BbePipeline bbePipeline{"./compiled_shaders/simple_shader.vert.spv", "./compiled_shaders/simple_shader.frag.spv"};
    };
} 