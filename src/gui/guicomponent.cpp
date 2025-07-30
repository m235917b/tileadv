#include <algorithm>

#include "gui/guicomponent.hpp"

GUIComponent::GUIComponent(const std::string &id)
    : id(id), rect({0.f, 0.f, 0.f, 0.f}), layout(GUILayout::FLOATING),
      type(GUIElementType::CONTAINER),
      style({0x606060FF, 0x303030FF, 0xFFFFFFFF, false, false, false, false,
             GUIFittingMode::HIDE, 1.f, 0.f}),
      visible(true), tempInvisible(false), navigable(true), children(),
      updateListener(), keyListeners(), mouseButtonListeners(), text(""),
      image(""), parent(nullptr), root(this) {}

GUIComponent::GUIComponent(const std::string &id, const float posX,
                           const float posY, const float width,
                           const float height)
    : id(id), rect({posX, posY, width, height}), layout(GUILayout::FLOATING),
      type(GUIElementType::CONTAINER),
      style({0x606060FF, 0x303030FF, 0xFFFFFFFF, false, false, false, false,
             GUIFittingMode::HIDE, 1.f, 0.f}),
      visible(true), tempInvisible(false), navigable(true), children(),
      updateListener(), keyListeners(), mouseButtonListeners(), text(""),
      image(""), parent(nullptr), root(this) {}

void GUIComponent::addChild(std::unique_ptr<GUIComponent> child) {
  child->parent = this;
  children.emplace(child->getId(), std::move(child));
  setRoot(parent ? root : this);
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
  tempInvisible = (rect.x < 0.f || rect.y < 0.f || rect.x + rect.w > 1.f ||
                   rect.y + rect.h > 1.f);

  if (tempInvisible) {
    return;
  }

  if (layout == GUILayout::VERTICAL) {
    float offset{style.spacing};

    auto topMargin{0.f};

    if (style.centerLeft) {
      auto sum{0.f};

      for (const auto &[_, child] : children) {
        sum += child->getHeight() + style.spacing;
      }

      sum += style.spacing;

      topMargin = (1.f - sum) / 2.f;
    }

    for (const auto &[_, child] : children) {
      if (style.centerTop) {
        const auto leftMargin{(1.f - child->getWidth()) / 2.f};
        child->rect.x = leftMargin;
      }

      child->rect.y = offset + topMargin;
      offset += child->getHeight() + style.spacing;
    }
  } else if (layout == GUILayout::HORIZONTAL) {
    float offset{style.spacing};

    auto leftMargin{0.f};

    if (style.centerTop) {
      auto sum{0.f};

      for (const auto &[_, child] : children) {
        sum += child->getWidth() + style.spacing;
      }

      sum += style.spacing;

      leftMargin = (1.f - sum) / 2.f;
    }

    for (const auto &[_, child] : children) {
      if (style.centerLeft) {
        const auto topMargin{(1.f - child->getHeight()) / 2.f};
        child->rect.y = topMargin;
      }

      child->rect.x = offset + leftMargin + style.spacing;
      offset += child->getWidth() + style.spacing;
    }
  }
}

void GUIComponent::keyDownListener(const SDL_Keycode key) {
  const auto it{keyListeners.find(key)};

  if (it != keyListeners.end()) {
    it->second();
  }
}

void GUIComponent::mouseButtonDownListener(const SDL_MouseButtonFlags button) {
  const auto it{mouseButtonListeners.find(button)};

  if (it != mouseButtonListeners.end()) {
    it->second();
  }
}

void GUIComponent::setUpdateListener(
    std::function<void(GUIComponent &component)> listener) {
  updateListener = std::move(listener);
}

void GUIComponent::addKeyListener(const SDL_Keycode key,
                                  std::function<void()> listener) {
  keyListeners[key] = std::move(listener);
}

void GUIComponent::addMouseButtonListener(const SDL_MouseButtonFlags button,
                                          std::function<void()> listener) {
  mouseButtonListeners[button] = std::move(listener);
}

void GUIComponent::setPosX(const float posX) { rect.x = posX; }

void GUIComponent::setPosY(const float posY) { rect.y = posY; }

void GUIComponent::setWidth(const float width) { rect.w = width; }

void GUIComponent::setHeight(const float height) { rect.h = height; }

void GUIComponent::setRect(SDL_FRect rect) { this->rect = rect; }

void GUIComponent::setBgColor(uint32_t rgba) { style.backgroundColor = rgba; }

void GUIComponent::setBdColor(uint32_t rgba) { style.borderColor = rgba; }

void GUIComponent::setTextColor(uint32_t rgba) { style.textColor = rgba; }

void GUIComponent::setBorder(const bool visible) { style.border = visible; }

void GUIComponent::setBackground(const bool visible) {
  style.background = visible;
}

void GUIComponent::setCenterLeft(const bool center) {
  style.centerLeft = center;
}

void GUIComponent::setCenterTop(const bool center) { style.centerTop = center; }

void GUIComponent::setFittingMode(const GUIFittingMode fittingMode) {
  style.fittingMode = fittingMode;
}

void GUIComponent::setScale(const float scale) { style.scale = scale; };

void GUIComponent::setSpacing(const float spacing) { style.spacing = spacing; }

void GUIComponent::setText(const std::string &text) { this->text = text; }

void GUIComponent::setImage(const std::string &path) { image = path; }

void GUIComponent::setLayout(const GUILayout layout) { this->layout = layout; }

void GUIComponent::setType(const GUIElementType type) { this->type = type; }

void GUIComponent::setVisible(const bool visible) { this->visible = visible; }

void GUIComponent::setNavigable(const bool navigable) {
  this->navigable = navigable;
}

std::string GUIComponent::getId() const { return id; }

float GUIComponent::getPosX() const { return rect.x; }

float GUIComponent::getPosY() const { return rect.y; }

float GUIComponent::getWidth() const { return rect.w; }

float GUIComponent::getHeight() const { return rect.h; }

const SDL_FRect &GUIComponent::getRect() const { return rect; }

const GUIStyle &GUIComponent::getStyle() const { return style; }

uint32_t GUIComponent::getBgColor() const { return style.backgroundColor; }

uint32_t GUIComponent::getBdColor() const { return style.borderColor; }

uint32_t GUIComponent::getTextColor() const { return style.textColor; }

bool GUIComponent::getBorder() const { return style.border; }

bool GUIComponent::getBackground() const { return style.background; }

bool GUIComponent::isCenteredLeft() const { return style.centerLeft; }

bool GUIComponent::isCenteredTop() const { return style.centerTop; }

GUIFittingMode GUIComponent::getFittingMode() const {
  return style.fittingMode;
}

float GUIComponent::getScale() const { return style.scale; }

float GUIComponent::getSpacing() const { return style.spacing; }

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

void GUIComponent::setRoot(GUIComponent *root) {
  this->root = root;

  for (auto &[_, child] : children) {
    child->setRoot(root);
  }
}
