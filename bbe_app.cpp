#include "bbe_app.hpp"

namespace bbe {
    void FirstApp::run() {
        while (!bbeWindow.shouldClose()) {
            glfwPollEvents();
        }
    }
}