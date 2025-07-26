#pragma once

#include <functional>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL3/SDL.h>

enum class GUILayout { FLOATING, VERTICAL, FULLSCREEN };

enum class GUIElementType { CONTAINER, TEXT };

class GUIComponent {
public:
  GUIComponent(const std::string &id);
  GUIComponent(const std::string &id, const int posX, const int posY,
               const int width, const int height);

  void addChild(std::unique_ptr<GUIComponent> child);
  bool removeChild(const std::string &id);
  void update();
  void updateLayout();
  void keyDownListener(const SDL_Keycode key);
  void setUpdateListener(std::function<void()> listener);
  void addKeyListener(const SDL_Keycode key, std::function<void()> listener);
  void forEachChild(std::function<void(GUIComponent &child)> action);

  void setPosX(const int posX);
  void setPosY(const int posY);
  void setBorder(const bool visible);
  void setBackground(const bool visible);
  void setSelected(const bool selected);
  void setActive(const bool active);
  void setText(const std::string &text);
  void setLayout(const GUILayout layout);
  void setType(const GUIElementType type);
  void setVisible(const bool visible);
  void selectNextChild();
  void selectPreviousChild();

  std::string getId() const;
  int getPosX() const;
  int getPosY() const;
  int getWidth() const;
  int getHeight() const;
  bool getBorder() const;
  bool getBackground() const;
  bool isSelected() const;
  bool isActive() const;
  std::string getText() const;
  GUILayout getLayout() const;
  GUIElementType getType() const;
  bool isVisible() const;
  const GUIComponent *getSelectedChild() const;

private:
  const std::string id;

  int posX;
  int posY;
  int width;
  int height;

  bool border;
  bool background;

  GUILayout layout;
  GUIElementType type;

  bool selected;
  bool active;
  bool visible;

  std::vector<std::unique_ptr<GUIComponent>> children;

  std::function<void()> updateListener;
  std::unordered_map<SDL_Keycode, std::function<void()>> keyListeners;

  int selectedChild;

  std::string text;

  void deselect();
};
