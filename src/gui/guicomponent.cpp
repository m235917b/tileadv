#include <algorithm>

#include "gui/guicomponent.hpp"

GUIComponent::GUIComponent(const std::string &id)
    : id(id), posX(0), posY(0), width(0), height(0), border(false),
      background(false), layout(GUILayout::FULLSCREEN),
      type(GUIElementType::CONTAINER), visible(true), children(),
      updateListener(), keyListeners(), text(""), image(""), parent(nullptr),
      root(nullptr) {}

GUIComponent::GUIComponent(const std::string &id, const int posX,
                           const int posY, const int width, const int height)
    : id(id), posX(posX), posY(posY), width(width), height(height),
      border(false), background(false), layout(GUILayout::FULLSCREEN),
      type(GUIElementType::CONTAINER), visible(true), children(),
      updateListener(), keyListeners(), text(""), image(""), parent(nullptr),
      root(nullptr) {}

void GUIComponent::addChild(std::unique_ptr<GUIComponent> child) {
  child->parent = this;
  child->root = parent ? this->root : this;
  children.emplace(child->getId(), std::move(child));
}

bool GUIComponent::removeChild(const std::string &id) {
  return children.erase(id) > 0;
}

void GUIComponent::update() {
  if (updateListener) {
    updateListener();
  }
}

void GUIComponent::updateLayout() {
  if (layout == GUILayout::VERTICAL) {
    int offset{0};

    for (const auto &[_, child] : children) {
      child->setPosX(posX + 10);
      child->setPosY(posY + 10 + offset);

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
  for (const auto &[_, child] : children) {
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

void GUIComponent::setText(const std::string &text) { this->text = text; }

void GUIComponent::setImage(const std::string &path) { image = path; }

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

std::string GUIComponent::getText() const { return text; }

std::string GUIComponent::getImage() const { return image; }

GUIComponent *GUIComponent::getParent() { return parent; }

GUIComponent *GUIComponent::getNextChild(const std::string &id) {
  if (children.empty()) {
    return nullptr;
  }

  auto it{children.find(id)};

  if (id == "" || it == children.end()) {
    return children.begin()->second.get();
  }

  it++;

  if (it == children.end()) {
    return children.begin()->second.get();
  }

  return it->second.get();
}

GUIComponent *GUIComponent::getPreviousChild(const std::string &id) {
  if (children.empty()) {
    return nullptr;
  }

  auto it{children.find(id)};

  if (id == "" || it == children.end()) {
    return children.begin()->second.get();
  } else if (it == children.begin()) {
    return (--children.end())->second.get();
  }

  return (--it)->second.get();
}

GUIComponent *GUIComponent::getRoot() { return root; }

int GUIComponent::numberOfChildren() const { return children.size(); }

GUILayout GUIComponent::getLayout() const { return layout; }

GUIElementType GUIComponent::getType() const { return type; }

bool GUIComponent::isVisible() const { return visible; }

bool GUIComponent::isDescendant(const std::string &id) {
  return std::all_of(children.begin(), children.end(),
                     [&id](const auto &pair) { return pair.first == id; });
}
