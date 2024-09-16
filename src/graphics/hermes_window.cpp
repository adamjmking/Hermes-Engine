#include "hermes_window.hpp"
#include <stdexcept>

namespace HermesEngine
{
    HermesWindow::HermesWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
    {
        initWindow();
    }

    HermesWindow::~HermesWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void HermesWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
    }

    void HermesWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window sufrace!");
        }
    }

    void HermesWindow::frameBufferResizedCallback(GLFWwindow *window, int width, int height)
    {
        auto hermesWindow = reinterpret_cast<HermesWindow *>(glfwGetWindowUserPointer(window));
        hermesWindow->frameBufferResized = true;
        hermesWindow->width = width;
        hermesWindow->height = height;
    }
}