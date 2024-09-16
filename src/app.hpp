#pragma once

#include "graphics/hermes_window.hpp"
#include "graphics/hermes_device.hpp"
#include "graphics/hermes_renderer.hpp"
#include "graphics/hermes_descriptors.hpp"

#include "game/hermes_game_object.hpp"

#include <memory>
#include <vector>

namespace HermesEngine
{
    class App
    {
    public:
        static constexpr int WIDTH = 1280;
        static constexpr int HEIGHT = 720;
        static constexpr float MAX_FRAME_TIME = 10.f;

        App();
        ~App();

        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void run();

    private:
        HermesWindow hermesWindow{WIDTH, HEIGHT, "Triathlon Sim"};
        HermesEngineDevice hermesDevice{hermesWindow};
        HermesRenderer hermesRenderer{hermesWindow, hermesDevice};

        std::unique_ptr<HermesDescriptorPool> globalPool{};
        HermesGameObject::Map gameObjects;

        void loadGameObjects();
    };
}