#include "gui/guiview.hpp"
#include "gui/asciiatlas.hpp"
#include "gui/guicomponent.hpp"
#include "utils/rendercontext.hpp"

GUIView::GUIView(const RenderContext &renderContext)
    : renderContext(renderContext), asciiGrey() {}

bool GUIView::init() {
  SDL_Renderer &renderer{renderContext.getRenderer()};

  if (asciiGrey.loadFromFile("guiassets/ascii_grey.png", renderer) == false) {
    SDL_Log("Unable to load png image!\n");

    return false;
  }

  return true;
}

void GUIView::drawGUIComponent(const GUIComponent &component) {
  SDL_Renderer &renderer{renderContext.getRenderer()};

  const SDL_FRect rect{static_cast<float>(component.getPosX()),
                       static_cast<float>(component.getPosY()),
                       static_cast<float>(component.getWidth()),
                       static_cast<float>(component.getHeight())};

  if (component.getBorder()) {
    SDL_SetRenderDrawColor(&renderer, 0x60, 0x60, 0x60, 0xFF);
    SDL_RenderRect(&renderer, &rect);
  }

  if (component.getBackground()) {
    SDL_SetRenderDrawColor(&renderer, 0x60, 0x60, 0x60, 0xFF);
    SDL_RenderFillRect(&renderer, &rect);
  }

  if (component.isSelected()) {
    if (component.getType() == GUIElementType::CONTAINER) {
      SDL_SetRenderDrawColor(&renderer, 0x50, 0x50, 0x50, 0xFF);
      SDL_RenderRect(&renderer, &rect);
    }
  }

  if (component.isActive()) {
    if (component.getType() == GUIElementType::CONTAINER) {
      SDL_SetRenderDrawColor(&renderer, 0xFF, 0x40, 0x40, 0xFF);
      SDL_RenderRect(&renderer, &rect);
    }
  }

  if (component.getType() == GUIElementType::TEXT) {
    const auto text{(component.isSelected() ? " > " : "   ") +
                    component.getText()};
    drawText(component.getPosX(), component.getPosY(), 1.f, text);
  }
}

void GUIView::drawText(const int posX, const int posY, const float size,
                       const std::string &text) {
  SDL_Renderer &renderer{renderContext.getRenderer()};

  for (size_t i = 0; i < text.length(); ++i) {
    const SDL_FRect spriteCoords = AsciiAtlas::getSpriteCoords(text[i]);
    float textPosX{posX + static_cast<float>(i) * asciiWidth * size};
    float textPosY{static_cast<float>(posY)};

    asciiGrey.render(textPosX, textPosY, spriteCoords, asciiWidth * size,
                     asciiHeight * size, renderer);
  }
}
