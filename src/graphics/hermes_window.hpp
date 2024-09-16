#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace HermesEngine
{
    class HermesWindow
    {
    public:
        HermesWindow(int width, int height, std::string windowName);
        ~HermesWindow();

        HermesWindow(const HermesWindow &) = delete;
        HermesWindow &operator=(const HermesWindow &) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

        bool wasWindowResized() { return frameBufferResized; }
        void resetWindowResizedFlag() { frameBufferResized = false; }
        GLFWwindow *getGLFWwindow() const { return window; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        int width;
        int height;
        bool frameBufferResized = false;

        std::string windowName;
        GLFWwindow *window;

        void initWindow();
        static void frameBufferResizedCallback(GLFWwindow *window, int width, int height);
    };
}