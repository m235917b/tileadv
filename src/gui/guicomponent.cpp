#include "gui/guicomponent.hpp"

GUIComponent::GUIComponent(const std::string& id) :
    id(id),
    posX(0),
    posY(0),
    width(0),
    height(0),
    border(false),
    background(false),
    layout(GUILayout::FULLSCREEN),
    type(GUIElementType::CONTAINER),
    selected(false),
    active(false),
    visible(true),
    items(),
    keyListeners(),
    updateListener(),
    selectedItem(-1),
    text("") {

}

GUIComponent::GUIComponent(const std::string& id, const int posX, const int posY, const int width, const int height) :
    id(id),
    posX(posX),
    posY(posY),
    width(width),
    height(height),
    border(false),
    background(false),
    layout(GUILayout::FULLSCREEN),
    type(GUIElementType::CONTAINER),
    selected(false),
    active(false),
    visible(true),
    items(),
    keyListeners(),
    updateListener(),
    selectedItem(-1),
    text("") {

}

void GUIComponent::deselect() {
    if (selectedItem != -1) {
        items[selectedItem]->setActive(false);
        items[selectedItem]->setSelected(false);

        for (const auto& item : items) {
            item->setActive(false);
            item->setSelected(false);
        }
    }

    selectedItem = -1;
}

void GUIComponent::addItem(std::unique_ptr<GUIComponent> item) {
    deselect();

    items.push_back(std::move(item));
}

bool GUIComponent::removeItem(const std::string& id) {
    deselect();

    return std::erase_if(items, [&id] (const std::unique_ptr<GUIComponent>& component) {
        return component->getId() == id;
    }) > 0;
}

void GUIComponent::update() {
    if (layout == GUILayout::VERTICAL) {
        int offset = 0;

        for (const auto& item : items) {
            item->setPosX(posX);
            item->setPosY(posY + offset);

            offset += item->getHeight();
        }
    }

    for (const auto& item : items) {
        item->update();
    }

    if (updateListener) {
        updateListener();
    }
}

void GUIComponent::keyDownListener(const SDL_Keycode key) {
    if (!selected) {
        return;
    }
    
    if (!active && key == SDLK_RETURN) {
        setActive(true);

        return;
    }

    auto it = keyListeners.find(key);
    if (it != keyListeners.end())
    {
        it->second();
    }

    if (!active) {
        return;
    }

    const auto childIsActive = selectedItem != -1 && items[selectedItem]->isActive();

    for (const auto& item : items) {
        item->keyDownListener(key);
    }

    if (childIsActive) {
        return;
    }

    if (layout == GUILayout::VERTICAL && key == SDLK_DOWN || layout != GUILayout::VERTICAL && key == SDLK_RIGHT) {
        selectNextItem();
    }

    if (layout == GUILayout::VERTICAL && key == SDLK_UP || layout != GUILayout::VERTICAL && key == SDLK_LEFT) {
        selectPreviousItem();
    }

    if (key == SDLK_BACKSPACE) {
        setActive(false);
    }
}

void GUIComponent::setPosX(const int posX) {
    this->posX = posX;
}

void GUIComponent::setPosY(const int posY) {
    this->posY = posY;
}

void GUIComponent::setUpdateListener(std::function<void()> listener) {
    updateListener = std::move(listener);
}

void GUIComponent::addKeyListener(const SDL_Keycode key, std::function<void()> listener) {
    keyListeners[key] = std::move(listener);
}

void GUIComponent::setBorder(const bool visible) {
    border = visible;
}

void GUIComponent::setBackground(const bool visible) {
    background = visible;
}

void GUIComponent::setSelected(const bool selected) {
    this->selected = selected;
}

void GUIComponent::setActive(const bool active) {
    this->active = active;
}

void GUIComponent::setText(const std::string& text) {
    this->text = text;
}

void GUIComponent::setLayout(const GUILayout layout) {
    this->layout = layout;
}

void GUIComponent::setType(const GUIElementType type) {
    this->type = type;
}

void GUIComponent::setVisible(const bool visible) {
    this->visible = visible;
}

std::string GUIComponent::getId() const {
    return id;
}

const std::vector<std::unique_ptr<GUIComponent>>& GUIComponent::getItems() const {
    return items;
}

int GUIComponent::getPosX() const {
    return posX;
}

int GUIComponent::getPosY() const {
    return posY;
}

int GUIComponent::getWidth() const {
    return width;
}

int GUIComponent::getHeight() const {
    return height;
}

bool GUIComponent::getBorder() const {
    return border;
}

bool GUIComponent::getBackground() const {
    return background;
}

bool GUIComponent::isSelected() const {
    return selected;
}

bool GUIComponent::isActive() const {
    return active;
}

std::string GUIComponent::getText() const {
    return text;
}

void GUIComponent::selectNextItem() {
    if (items.empty()) {
        return;
    }

    if (selectedItem != -1) {
        items[selectedItem]->setSelected(false);
    }

    selectedItem++;

    if (selectedItem >= items.size()) {
        selectedItem = 0;
    }

    items[selectedItem]->setSelected(true);
}

void GUIComponent::selectPreviousItem() {
    if (items.empty()) {
        return;
    }

    if (selectedItem != -1) {
        items[selectedItem]->setSelected(false);
    }

    selectedItem--;

    if (selectedItem < 0) {
        selectedItem = items.size() - 1;
    }

    items[selectedItem]->setSelected(true);
}

GUILayout GUIComponent::getLayout() const {
    return layout;
}

GUIElementType GUIComponent::getType() const {
    return type;
}

bool GUIComponent::isVisible() const {
    return visible;
}
