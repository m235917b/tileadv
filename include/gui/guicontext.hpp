#pragma once

#include <vector>
#include <string>
#include <memory>
#include <set>
#include <functional>

#include <SDL3/SDL.h>

#include "gui/guiview.hpp"
#include "gui/guicomponent.hpp"

class RenderContext;

class GUIContext {
    public:
        GUIContext(const RenderContext& renderContext);

        bool init();

        void keyDownListener(const SDL_Keycode key);
        void addKeyListener(const std::string& id, const SDL_Keycode key, std::function<void()> listener);

        void addComponent(std::unique_ptr<GUIComponent> component);
        bool removeComponent(const std::string& id);

        void setComponentVisible(const std::string& id, const bool visible);

        void update();
        void drawGUI();

    private:
        GUIView guiView;

        std::vector<std::unique_ptr<GUIComponent>> components;
};