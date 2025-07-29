#include <algorithm>

#include "gui/guicomponent.hpp"

GUIComponent::GUIComponent(const std::string &id)
    : id(id), posX(0.f), posY(0.f), width(0.f), height(0.f), border(false),
      background(false), centerLeft(false), centerTop(false),
      fittingMode(GUIFittingMode::HIDE), scale(1.f), spacing(0.f),
      layout(GUILayout::FLOATING), type(GUIElementType::CONTAINER),
      visible(true), tempInvisible(false), navigable(true), children(),
      updateListener(), keyListeners(), text(""), image(""), parent(nullptr),
      root(this) {}

GUIComponent::GUIComponent(const std::string &id, const float posX,
                           const float posY, const float width,
                           const float height)
    : id(id), posX(posX), posY(posY), width(width), height(height),
      border(false), background(false), centerLeft(false), centerTop(false),
      fittingMode(GUIFittingMode::HIDE), scale(1.f), spacing(0.f),
      layout(GUILayout::FLOATING), type(GUIElementType::CONTAINER),
      visible(true), tempInvisible(false), navigable(true), children(),
      updateListener(), keyListeners(), text(""), image(""), parent(nullptr),
      root(this) {}

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
    updateListener(*this);
  }
}

void GUIComponent::updateLayout() {
  tempInvisible =
      (posX < 0.f || posY < 0.f || posX + width > 1.f || posY + height > 1.f);

  if (tempInvisible) {
    return;
  }

  if (layout == GUILayout::VERTICAL) {
    float offset{spacing};

    auto topMargin{0.f};

    if (centerLeft) {
      auto sum{0.f};

      for (const auto &[_, child] : children) {
        sum += child->getHeight() + spacing;
      }

      sum += spacing;

      topMargin = (1.f - sum) / 2.f;
    }

    for (const auto &[_, child] : children) {
      if (centerTop) {
        const auto leftMargin{(1.f - child->getWidth()) / 2.f};
        child->posX = leftMargin;
      }

      child->posY = offset + topMargin;
      offset += child->getHeight() + spacing;
    }
  } else if (layout == GUILayout::HORIZONTAL) {
    float offset{spacing};

    auto leftMargin{0.f};

    if (centerTop) {
      auto sum{0.f};

      for (const auto &[_, child] : children) {
        sum += child->getWidth() + spacing;
      }

      sum += spacing;

      leftMargin = (1.f - sum) / 2.f;
    }

    for (const auto &[_, child] : children) {
      if (centerLeft) {
        const auto topMargin{(1.f - child->getHeight()) / 2.f};
        child->posY = topMargin;
      }

      child->posX = offset + leftMargin + spacing;
      offset += child->getWidth() + spacing;
    }
  }
}

void GUIComponent::keyDownListener(const SDL_Keycode key) {
  const auto it{keyListeners.find(key)};

  if (it != keyListeners.end()) {
    it->second();
  }
}

void GUIComponent::setPosX(const float posX) { this->posX = posX; }

void GUIComponent::setPosY(const float posY) { this->posY = posY; }

void GUIComponent::setWidth(const float width) { this->width = width; }

void GUIComponent::setHeight(const float height) { this->height = height; }

void GUIComponent::setUpdateListener(
    std::function<void(GUIComponent &component)> listener) {
  updateListener = std::move(listener);
}

void GUIComponent::addKeyListener(const SDL_Keycode key,
                                  std::function<void()> listener) {
  keyListeners[key] = std::move(listener);
}

void GUIComponent::setBorder(const bool visible) { border = visible; }

void GUIComponent::setBackground(const bool visible) { background = visible; }

void GUIComponent::setCenterLeft(const bool center) { centerLeft = center; }

void GUIComponent::setCenterTop(const bool center) { centerTop = center; }

void GUIComponent::setFittingMode(const GUIFittingMode fittingMode) {
  this->fittingMode = fittingMode;
}

void GUIComponent::setScale(const float scale) { this->scale = scale; };

void GUIComponent::setSpacing(const float spacing) { this->spacing = spacing; }

void GUIComponent::setText(const std::string &text) { this->text = text; }

void GUIComponent::setImage(const std::string &path) { image = path; }

void GUIComponent::setLayout(const GUILayout layout) { this->layout = layout; }

void GUIComponent::setType(const GUIElementType type) { this->type = type; }

void GUIComponent::setVisible(const bool visible) { this->visible = visible; }

void GUIComponent::setNavigable(const bool navigable) {
  this->navigable = navigable;
}

std::string GUIComponent::getId() const { return id; }

float GUIComponent::getPosX() const { return posX; }

float GUIComponent::getPosY() const { return posY; }

float GUIComponent::getWidth() const { return width; }

float GUIComponent::getHeight() const { return height; }

bool GUIComponent::getBorder() const { return border; }

bool GUIComponent::getBackground() const { return background; }

bool GUIComponent::isCenteredLeft() const { return centerLeft; }

bool GUIComponent::isCenteredTop() const { return centerTop; }

GUIFittingMode GUIComponent::getFittingMode() const { return fittingMode; }

float GUIComponent::getScale() const { return scale; }

float GUIComponent::getSpacing() const { return spacing; }

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

bool GUIComponent::isVisible() const { return visible && !tempInvisible; }

bool GUIComponent::isNavigable() const { return navigable; }

bool GUIComponent::isDescendant(const std::string &id) {
  if (this->id == id) {
    return true;
  }

  for (const auto &[_, child] : children) {
    if (child->isDescendant(id)) {
      return true;
    }
  }

  return false;
}
