#pragma once

#include "bbe_window.hpp"
#include "bbe_pipeline.hpp"
#include "bbe_device.hpp"
namespace bbe {
    class FirstApp {
        public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        void run();
        private:
        BbeWindow bbeWindow{WIDTH, HEIGHT, "Hello, Vulkan!"};
        BbeDevice bbeDevice{bbeWindow};
        BbePipeline bbePipeline{bbeDevice, "./compiled_shaders/simple_shader.vert.spv", "./compiled_shaders/simple_shader.frag.spv", BbePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
} 