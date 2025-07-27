#include "gui/guiview.hpp"
#include "gui/asciiatlas.hpp"
#include "gui/guicomponent.hpp"
#include "gui/guitreewalker.hpp"
#include "utils/rendercontext.hpp"

GUIView::GUIView(const RenderContext &renderContext)
    : renderContext(renderContext), asciiGrey(), borderMargin(.02f) {}

void GUIView::drawGUIComponent(GUIComponent &component,
                               const std::string &selected) {

  auto action{[this, &selected](GUIComponent &node, float &posXParentAbs,
                                float &posYParentAbs, float &widthParent,
                                float &heightParent) {
    if (!node.isVisible()) {
      return false;
    }

    SDL_Renderer &renderer{renderContext.getRenderer()};
    const auto screenWidth{renderContext.getScreenWidth()};
    const auto screenHeight{renderContext.getScreenHeight()};

    const SDL_FRect rect = {
        posXParentAbs +
            (node.getPosX() + borderMargin) * widthParent * screenWidth,
        posYParentAbs +
            (node.getPosY() + borderMargin) * heightParent * screenHeight,
        (node.getWidth() - 2.f * borderMargin) * widthParent * screenWidth,
        (node.getHeight() - 2.f * borderMargin) * heightParent * screenHeight};

    if (node.getBorder()) {
      SDL_SetRenderDrawColor(&renderer, 0x60, 0x60, 0x60, 0xFF);
      SDL_RenderRect(&renderer, &rect);
    }

    if (node.getBackground()) {
      SDL_SetRenderDrawColor(&renderer, 0x60, 0x60, 0x60, 0xFF);
      SDL_RenderFillRect(&renderer, &rect);
    }

    if (!node.getText().empty()) {
      const auto text{(selected == node.getId() ? " > " : "   ") +
                      node.getText() + "   "};
      drawText(rect.x, rect.y, rect.w, rect.h, text);
    } else if (selected == node.getId()) {
      SDL_SetRenderDrawColor(&renderer, 0xFF, 0x40, 0x40, 0x20);
      SDL_RenderFillRect(&renderer, &rect);
    }

    if (!node.getImage().empty()) {
      drawImage(rect.x, rect.y, rect.w, rect.h, node.getImage());
    }

    posXParentAbs = rect.x;
    posYParentAbs = rect.y;
    widthParent = (node.getWidth() - 2.f * borderMargin) * widthParent;
    heightParent = (node.getHeight() - 2.f * borderMargin) * heightParent;

    return true;
  }};

  GUITreeWalker::traverse(component, action, 0.f, 0.f, 1.f, 1.f);
}

void GUIView::drawText(const float posX, const float posY, const float width,
                       const float height, const std::string &text) {
  SDL_Renderer &renderer{renderContext.getRenderer()};
  const auto charWidth{width / static_cast<float>(text.length())};

  for (size_t i = 0; i < text.length(); ++i) {
    const SDL_FRect spriteCoords = AsciiAtlas::getSpriteCoords(text[i]);
    float textPosX{posX + static_cast<float>(i) * charWidth};

    SDL_SetRenderDrawColor(&renderer, 0x50, 0x50, 0x50, 0xFF);

    asciiGrey.render(textPosX, posY, &spriteCoords, charWidth, height,
                     renderer);
  }
}

void GUIView::drawImage(const float posX, const float posY, const float width,
                        const float height, const std::string &path) {
  const auto &texture{images.find(path)};

  if (texture == images.end()) {
    return;
  }

  SDL_Renderer &renderer{renderContext.getRenderer()};

  texture->second.render(posX, posY, nullptr, width, height, renderer);
}
