#include "gui/guicomponent.hpp"

GUIComponent::GUIComponent(const std::string &id)
    : id(id), posX(0), posY(0), width(0), height(0), border(false),
      background(false), layout(GUILayout::FULLSCREEN),
      type(GUIElementType::CONTAINER), selected(false), active(false),
      visible(true), children(), updateListener(), keyListeners(),
      selectedChild(-1), text("") {}

GUIComponent::GUIComponent(const std::string &id, const int posX,
                           const int posY, const int width, const int height)
    : id(id), posX(posX), posY(posY), width(width), height(height),
      border(false), background(false), layout(GUILayout::FULLSCREEN),
      type(GUIElementType::CONTAINER), selected(false), active(false),
      visible(true), children(), updateListener(), keyListeners(),
      selectedChild(-1), text("") {}

void GUIComponent::deselect() {
  if (selectedChild != -1) {
    children[selectedChild]->setActive(false);
    children[selectedChild]->setSelected(false);
  }

  selectedChild = -1;
}

void GUIComponent::addChild(std::unique_ptr<GUIComponent> child) {
  deselect();

  children.push_back(std::move(child));
}

bool GUIComponent::removeChild(const std::string &id) {
  deselect();

  return std::erase_if(children,
                       [&id](const std::unique_ptr<GUIComponent> &component) {
                         return component->getId() == id;
                       }) > 0;
}

void GUIComponent::update() {
  if (updateListener) {
    updateListener();
  }
}

void GUIComponent::updateLayout() {
  if (layout == GUILayout::VERTICAL) {
    int offset{0};

    for (const auto &child : children) {
      child->setPosX(posX);
      child->setPosY(posY + offset);

      offset += child->getHeight();
    }
  }
}

void GUIComponent::keyDownListener(const SDL_Keycode key) {
  const auto it{keyListeners.find(key)};

  if (it != keyListeners.end()) {
    it->second();
  }
}

void GUIComponent::forEachChild(
    std::function<void(GUIComponent &child)> action) {
  for (const auto &child : children) {
    action(*child);
  }
}

void GUIComponent::setPosX(const int posX) { this->posX = posX; }

void GUIComponent::setPosY(const int posY) { this->posY = posY; }

void GUIComponent::setUpdateListener(std::function<void()> listener) {
  updateListener = std::move(listener);
}

void GUIComponent::addKeyListener(const SDL_Keycode key,
                                  std::function<void()> listener) {
  keyListeners[key] = std::move(listener);
}

void GUIComponent::setBorder(const bool visible) { border = visible; }

void GUIComponent::setBackground(const bool visible) { background = visible; }

void GUIComponent::setSelected(const bool selected) {
  this->selected = selected;
}

void GUIComponent::setActive(const bool active) {
  this->active = active;

  if (!active) {
    deselect();
  }
}

void GUIComponent::setText(const std::string &text) { this->text = text; }

void GUIComponent::setLayout(const GUILayout layout) { this->layout = layout; }

void GUIComponent::setType(const GUIElementType type) { this->type = type; }

void GUIComponent::setVisible(const bool visible) { this->visible = visible; }

std::string GUIComponent::getId() const { return id; }

int GUIComponent::getPosX() const { return posX; }

int GUIComponent::getPosY() const { return posY; }

int GUIComponent::getWidth() const { return width; }

int GUIComponent::getHeight() const { return height; }

bool GUIComponent::getBorder() const { return border; }

bool GUIComponent::getBackground() const { return background; }

bool GUIComponent::isSelected() const { return selected; }

bool GUIComponent::isActive() const { return active; }

std::string GUIComponent::getText() const { return text; }

const GUIComponent *GUIComponent::getSelectedChild() const {
  if (selectedChild != -1) {
    return children[selectedChild].get();
  }

  return nullptr;
}

void GUIComponent::selectNextChild() {
  if (children.empty()) {
    return;
  }

  if (selectedChild != -1) {
    children[selectedChild]->setSelected(false);
  }

  selectedChild++;

  if (static_cast<size_t>(selectedChild) >= children.size()) {
    selectedChild = 0;
  }

  children[selectedChild]->setSelected(true);
}

void GUIComponent::selectPreviousChild() {
  if (children.empty()) {
    return;
  }

  if (selectedChild != -1) {
    children[selectedChild]->setSelected(false);
  }

  selectedChild--;

  if (selectedChild < 0) {
    selectedChild = children.size() - 1;
  }

  children[selectedChild]->setSelected(true);
}

GUILayout GUIComponent::getLayout() const { return layout; }

GUIElementType GUIComponent::getType() const { return type; }

bool GUIComponent::isVisible() const { return visible; }
