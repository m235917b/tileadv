#pragma once

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
  void setUpdateListener(std::function<void()> listener);
  void addKeyListener(const SDL_Keycode key, std::function<void()> listener);
  void forEachChild(std::function<void(GUIComponent &child)> action);

  void setPosX(const float posX);
  void setPosY(const float posY);
  void setWidth(const float width);
  void setHeight(const float height);
  void setBorder(const bool visible);
  void setBackground(const bool visible);
  void setCenterLeft(const bool center);
  void setCenterTop(const bool center);
  void setFittingMode(const GUIFittingMode fittingMode);
  void setScale(const float scale);
  void setText(const std::string &text);
  void setImage(const std::string &path);
  void setLayout(const GUILayout layout);
  void setType(const GUIElementType type);
  void setVisible(const bool visible);

  std::string getId() const;
  float getPosX() const;
  float getPosY() const;
  float getWidth() const;
  float getHeight() const;
  bool getBorder() const;
  bool getBackground() const;
  bool isCenteredLeft() const;
  bool isCenteredTop() const;
  GUIFittingMode getFittingMode() const;
  float getScale() const;
  std::string getText() const;
  std::string getImage() const;
  GUILayout getLayout() const;
  GUIElementType getType() const;
  bool isVisible() const;
  GUIComponent *getParent();
  GUIComponent *getNextChild(const std::string &id);
  GUIComponent *getPreviousChild(const std::string &id);
  GUIComponent *getRoot();
  int numberOfChildren() const;
  bool isDescendant(const std::string &id);

private:
  const std::string id;

  float posX;
  float posY;
  float width;
  float height;

  bool border;
  bool background;
  bool centerLeft;
  bool centerTop;
  GUIFittingMode fittingMode;
  float scale;

  GUILayout layout;
  GUIElementType type;

  bool visible;

  std::map<std::string, std::unique_ptr<GUIComponent>> children;

  std::function<void()> updateListener;
  std::unordered_map<SDL_Keycode, std::function<void()>> keyListeners;

  std::string text;
  std::string image;

  GUIComponent *parent;
  GUIComponent *root;
};
