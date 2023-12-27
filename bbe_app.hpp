#pragma once

#include "bbe_window.hpp"

namespace bbe {
    class FirstApp {
        public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        void run();
        private:
        BbeWindow bbeWindow{WIDTH, HEIGHT, "Hello, Vulkan!"};
    };
}