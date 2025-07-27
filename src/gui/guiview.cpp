#include "gui/guiview.hpp"
#include "gui/asciiatlas.hpp"
#include "gui/guicomponent.hpp"
#include "utils/rendercontext.hpp"

GUIView::GUIView(const RenderContext &renderContext)
    : renderContext(renderContext), asciiGrey() {}

bool GUIView::init(std::vector<std::string> texturePaths) {
  SDL_Renderer &renderer{renderContext.getRenderer()};

  SDL_SetRenderDrawBlendMode(&renderer, SDL_BLENDMODE_BLEND);

  if (!asciiGrey.loadFromFile("guiassets/ascii_grey.png", renderer)) {
    SDL_Log("Unable to load png image!\n");

    return false;
  }

  for (const auto &path : texturePaths) {
    if (images.try_emplace(path).second) {
      if (!images[path].loadFromFile(path, renderer)) {
        return false;
      }
    }
  }

  return true;
}

void GUIView::drawGUIComponent(const GUIComponent &component,
                               const bool selected) {
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

  if (!component.getText().empty()) {
    const auto text{(selected ? " > " : "   ") + component.getText()};
    drawText(component.getPosX(), component.getPosY(), 1.f, text);
  } else if (selected) {
    SDL_SetRenderDrawColor(&renderer, 0xFF, 0x40, 0x40, 0x20);
    SDL_RenderFillRect(&renderer, &rect);
  }

  if (!component.getImage().empty()) {
    drawImage(component.getPosX(), component.getPosY(), component.getWidth(),
              component.getHeight(), component.getImage());
  }
}

void GUIView::drawText(const int posX, const int posY, const float size,
                       const std::string &text) {
  SDL_Renderer &renderer{renderContext.getRenderer()};

  for (size_t i = 0; i < text.length(); ++i) {
    const SDL_FRect spriteCoords = AsciiAtlas::getSpriteCoords(text[i]);
    float textPosX{posX + static_cast<float>(i) * asciiWidth * size};
    float textPosY{static_cast<float>(posY)};

    SDL_SetRenderDrawColor(&renderer, 0x50, 0x50, 0x50, 0xFF);

    asciiGrey.render(textPosX, textPosY, &spriteCoords, asciiWidth * size,
                     asciiHeight * size, renderer);
  }
}

void GUIView::drawImage(const int posX, const int posY, const int width,
                        const int height, const std::string &path) {
  const auto &texture{images.find(path)};

  if (texture == images.end()) {
    return;
  }

  SDL_Renderer &renderer{renderContext.getRenderer()};

  texture->second.render(posX, posY, nullptr, width, height, renderer);
}
