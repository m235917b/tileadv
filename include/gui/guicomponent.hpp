#pragma once

#include <functional>
#include <map>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_map>


#include <SDL3/SDL.h>

enum class GUILayout { FLOATING, VERTICAL, FULLSCREEN };

enum class GUIElementType { CONTAINER, ELEMENT };

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
  void setText(const std::string &text);
  void setImage(const std::string &path);
  void setLayout(const GUILayout layout);
  void setType(const GUIElementType type);
  void setVisible(const bool visible);

  std::string getId() const;
  int getPosX() const;
  int getPosY() const;
  int getWidth() const;
  int getHeight() const;
  bool getBorder() const;
  bool getBackground() const;
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

  int posX;
  int posY;
  int width;
  int height;

  bool border;
  bool background;

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
