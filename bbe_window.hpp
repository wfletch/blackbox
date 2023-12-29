#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace bbe {
    class BbeWindow {
        public:
            BbeWindow(int w, int h, std::string name);
            ~BbeWindow();

            BbeWindow(const BbeWindow &) = delete;
            BbeWindow &operator=(const BbeWindow &) = delete;

            bool shouldClose(){return glfwWindowShouldClose(window); };
            VkExtent2D getExtent() {return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};}
            void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

            bool wasWindowResized() {return frameBufferResized;}
            void resetWindowResizedFlag(){frameBufferResized = false;}


        private:
        void initWindow();
        static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
        int width;
        int height;
        bool frameBufferResized = false;
        std::string windowName;
        GLFWwindow *window;
        

    };
}
// End of namespace bbe