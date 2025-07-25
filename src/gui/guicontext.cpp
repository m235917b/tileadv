#include "gui/guicontext.hpp"
#include "utils/rendercontext.hpp"
#include "gui/asciiatlas.hpp"
#include "gui/guitreetraverser.hpp"

GUIContext::GUIContext(const RenderContext& renderContext)
    : guiView(renderContext), components() {
    
}

bool GUIContext::init() {
    return guiView.init();
}

void GUIContext::keyDownListener(const SDL_Keycode key) {
    /*GUITreeTraverser::traverse(components, [key] (const std::unique_ptr<GUIComponent>& component) {
        if (component->isVisible()) {
            component->keyDownListener(key);
        }
    });*/

    for (const auto& component : components) {
        component->keyDownListener(key);
    }
}

void GUIContext::addKeyListener(const std::string& id, const SDL_Keycode key, std::function<void()> listener) {
    GUITreeTraverser::traverse(components, [&id, key, listener] (const std::unique_ptr<GUIComponent>& component) {
        if (component->getId() == id) {
            component->addKeyListener(key, listener);
        }
    });
}

void GUIContext::addComponent(std::unique_ptr<GUIComponent> component) {
    components.push_back(std::move(component));
}

bool GUIContext::removeComponent(const std::string& id) {
    return std::erase_if(components, [&id] (const std::unique_ptr<GUIComponent>& component) {
        return component->getId() == id;
    });
}

void GUIContext::setComponentVisible(const std::string& id, const bool visible) {
    GUITreeTraverser::traverse(components, [&id, visible] (const std::unique_ptr<GUIComponent>& component) {
        if (component->getId() == id) {
            component->setVisible(visible);
            component->setActive(visible);
            component->setSelected(visible);
        }
    });
}

void GUIContext::drawGUI() {
    for (const auto& component : components) {
        if (component->isVisible()) {
            GUITreeTraverser::traverse(component->getItems(), [this] (const std::unique_ptr<GUIComponent>& component) {
                this->guiView.drawGUIComponent(*component);
            });
        }
    }
}

void GUIContext::update() {
    for (const auto& component : components) {
        if (component->isVisible()) {
            GUITreeTraverser::traverse(component->getItems(), [] (const std::unique_ptr<GUIComponent>& component) {
                component->update();
            });
        }
    }
}
