#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

#include <SDL3/SDL.h>

enum class GUILayout {
    FLOATING,
    VERTICAL,
    FULLSCREEN
};

enum class GUIElementType {
    CONTAINER,
    TEXT
};

class GUIComponent {
    public:
        GUIComponent(const std::string& id);
        GUIComponent(const std::string& id, const int posX, const int posY, const int width, const int height);

        void addItem(std::unique_ptr<GUIComponent> item);
        bool removeItem(const std::string& id);
        void update();
        void keyDownListener(const SDL_Keycode key);
        void setUpdateListener(std::function<void()> listener);
        void addKeyListener(const SDL_Keycode key, std::function<void()> listener);

        void setPosX(const int posX);
        void setPosY(const int posY);
        void setBorder(const bool visible);
        void setBackground(const bool visible);
        void setSelected(const bool selected);
        void setActive(const bool active);
        void setText(const std::string& text);
        void setLayout(const GUILayout layout);
        void setType(const GUIElementType type);
        void setVisible(const bool visible);

        std::string getId() const;
        const std::vector<std::unique_ptr<GUIComponent>>& getItems() const;
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

        std::vector<std::unique_ptr<GUIComponent>> items;

        std::function<void()> updateListener;
        std::unordered_map<SDL_Keycode, std::function<void()>> keyListeners;

        int selectedItem;

        void selectNextItem();
        void selectPreviousItem();

        std::string text;

        void deselect();
};
