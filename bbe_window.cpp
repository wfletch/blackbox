#include "bbe_window.hpp"
#include <string>
namespace bbe {
    BbeWindow::BbeWindow(int w, int h, std::string name) : width{w}, height {h}, windowName{name} {
        initWindow();
    }
    BbeWindow::~BbeWindow(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void BbeWindow::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }
}