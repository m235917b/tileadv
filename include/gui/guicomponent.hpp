#pragma once

#include <concepts>
#include <functional>
#include <map>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>

enum class GUILayout { FLOATING, VERTICAL, HORIZONTAL };

enum class GUIElementType { CONTAINER, ELEMENT };

enum class GUIFittingMode { CLIP, SCALE, HIDE };

struct GUIStyle {
  uint32_t backgroundColor;
  uint32_t borderColor;
  uint32_t textColor;
  bool border;
  bool background;
  bool centerLeft;
  bool centerTop;
  GUIFittingMode fittingMode;
  float scale;
  float spacing;
};

class GUIComponent {
public:
  GUIComponent(const std::string &id);
  GUIComponent(const std::string &id, const float posX, const float posY,
               const float width, const float height);

  void addChild(std::unique_ptr<GUIComponent> child);
  bool removeChild(const std::string &id);
  void update();
  void updateLayout();
  void keyDownListener(const SDL_Keycode key);
  void mouseButtonDownListener(const SDL_MouseButtonFlags button);
  void setUpdateListener(std::function<void(GUIComponent &component)> listener);
  void addKeyListener(const SDL_Keycode key, std::function<void()> listener);
  void addMouseButtonListener(const SDL_MouseButtonFlags button,
                              std::function<void()> listener);

  template <typename Func>
    requires std::invocable<Func, GUIComponent &>
  void forEachChild(Func &&action) {
    for (const auto &[_, child] : children) {
      std::invoke(std::forward<Func>(action), *child);
    }
  }

  void setPosX(const float posX);
  void setPosY(const float posY);
  void setWidth(const float width);
  void setHeight(const float height);
  void setRect(SDL_FRect rect);
  void setBgColor(uint32_t rgba);
  void setBdColor(uint32_t rgba);
  void setTextColor(uint32_t rgba);
  void setBorder(const bool visible);
  void setBackground(const bool visible);
  void setCenterLeft(const bool center);
  void setCenterTop(const bool center);
  void setFittingMode(const GUIFittingMode fittingMode);
  void setScale(const float scale);
  void setSpacing(const float spacing);
  void setStyle(const GUIStyle style);
  void setText(const std::string &text);
  void setImage(const std::string &path);
  void setLayout(const GUILayout layout);
  void setType(const GUIElementType type);
  void setVisible(const bool visible);
  void setNavigable(const bool navigable);

  std::string getId() const;
  float getPosX() const;
  float getPosY() const;
  float getWidth() const;
  float getHeight() const;
  const SDL_FRect &getRect() const;
  const GUIStyle &getStyle() const;
  uint32_t getBgColor() const;
  uint32_t getBdColor() const;
  uint32_t getTextColor() const;
  bool getBorder() const;
  bool getBackground() const;
  bool isCenteredLeft() const;
  bool isCenteredTop() const;
  GUIFittingMode getFittingMode() const;
  float getScale() const;
  float getSpacing() const;
  std::string getText() const;
  std::string getImage() const;
  GUILayout getLayout() const;
  GUIElementType getType() const;
  bool isVisible() const;
  bool isNavigable() const;
  GUIComponent *getParent();
  GUIComponent *getNextChild(const std::string &id);
  GUIComponent *getPreviousChild(const std::string &id);
  GUIComponent *getRoot();
  int numberOfChildren() const;
  bool isDescendant(const std::string &id);

private:
  const std::string id;

  SDL_FRect rect;

  GUILayout layout;
  GUIElementType type;

  GUIStyle style;

  bool visible;
  bool tempInvisible;
  bool navigable;

  std::map<std::string, std::unique_ptr<GUIComponent>> children;

  std::function<void(GUIComponent &component)> updateListener;
  std::unordered_map<SDL_Keycode, std::function<void()>> keyListeners;
  std::unordered_map<SDL_MouseButtonFlags, std::function<void()>>
      mouseButtonListeners;

  std::string text;
  std::string image;

  GUIComponent *parent;
  GUIComponent *root;

  void setRoot(GUIComponent *root);
};
