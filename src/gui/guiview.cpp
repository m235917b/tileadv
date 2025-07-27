#include <cmath>

#include "gui/asciiatlas.hpp"
#include "gui/guicomponent.hpp"
#include "gui/guitreewalker.hpp"
#include "gui/guiview.hpp"
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

    auto &renderer{renderContext.getRenderer()};
    const auto screenWidth{renderContext.getScreenWidth()};
    const auto screenHeight{renderContext.getScreenHeight()};

    const SDL_FRect rect{
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
      const auto text{(selected == node.getId() ? ">" : " ") + node.getText() +
                      " "};
      drawText(rect.x, rect.y, rect.w, rect.h, text, node.getScale(),
               node.getFittingMode(), node.isCenteredLeft(),
               node.isCenteredTop());
    } else if (selected == node.getId()) {
      SDL_SetRenderDrawColor(&renderer, 0xFF, 0x40, 0x40, 0x20);
      SDL_RenderFillRect(&renderer, &rect);
    }

    if (!node.getImage().empty()) {
      drawImage(rect.x, rect.y, rect.w, rect.h, node.getImage(),
                node.getScale(), node.getFittingMode(), node.isCenteredLeft(),
                node.isCenteredTop());
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
                       const float height, const std::string &text,
                       const float size, GUIFittingMode fittingMode,
                       const bool centerLeft, const bool centerTop) {
  auto &renderer{renderContext.getRenderer()};

  const auto charWidth{fittingMode == GUIFittingMode::SCALE
                           ? width / static_cast<float>(text.length())
                           : asciiWidth * size};
  const auto charHeight{asciiHeight * size};
  const auto leftMargin{
      centerTop && fittingMode != GUIFittingMode::SCALE
          ? std::max(0.f, (width - text.length() * charWidth) / 2.f)
          : 0.f};
  const auto topMargin{centerLeft && fittingMode != GUIFittingMode::SCALE
                           ? std::max(0.f, (height - charHeight) / 2.f)
                           : 0.f};
  const auto textPosY{posY + topMargin};
  const auto charHeightClipped{
      fittingMode == GUIFittingMode::CLIP
          ? std::max(0.f, std::min(charHeight, posY + height - textPosY))
          : (fittingMode == GUIFittingMode::SCALE ? height : charHeight)};

  for (size_t i = 0; i < text.length(); ++i) {
    auto spriteCoords{AsciiAtlas::getSpriteCoords(text[i])};
    const auto textPosX{posX + static_cast<float>(i) * charWidth + leftMargin};

    const auto charWidthClipped{
        fittingMode == GUIFittingMode::CLIP
            ? std::max(0.f, std::min(charWidth, posX + width - textPosX))
            : charWidth};

    if (fittingMode == GUIFittingMode::CLIP) {
      spriteCoords.w = spriteCoords.w * (charWidthClipped / charWidth);
      spriteCoords.h = spriteCoords.h * (charHeightClipped / charHeight);
    }

    if (textPosX + charWidthClipped <= posX + width &&
        textPosY + charHeightClipped <= posY + height) {
      SDL_SetRenderDrawColor(&renderer, 0x50, 0x50, 0x50, 0xFF);
      asciiGrey.render(textPosX, textPosY, &spriteCoords, charWidthClipped,
                       charHeightClipped, renderer);
    }
  }
}

void GUIView::drawImage(const float posX, const float posY, const float width,
                        const float height, const std::string &path,
                        const float size, GUIFittingMode fittingMode,
                        const bool centerLeft, const bool centerTop) {
  const auto &texture{images.find(path)};

  if (texture == images.end()) {
    return;
  }

  const auto imgWidth{static_cast<float>(texture->second.getWidth()) * size};
  const auto imgHeight{static_cast<float>(texture->second.getHeight()) * size};
  const auto imgPosX{centerTop ? posX + std::max(0.f, (width - imgWidth) / 2.f)
                               : posX};
  const auto imgPosY{
      centerLeft ? posY + std::max(0.f, (height - imgHeight) / 2.f) : posY};
  const auto imgWidthClipped{
      fittingMode == GUIFittingMode::CLIP
          ? std::max(0.f, std::min(imgWidth, posX + width - imgPosX))
          : (fittingMode == GUIFittingMode::SCALE ? width : imgWidth)};
  const auto imgHeightClipped{
      fittingMode == GUIFittingMode::CLIP
          ? std::max(0.f, std::min(imgHeight, posY + height - imgPosY))
          : (fittingMode == GUIFittingMode::SCALE ? height : imgHeight)};

  const SDL_FRect rect{0.f, 0.f,
                       fittingMode == GUIFittingMode::CLIP
                           ? static_cast<float>(texture->second.getWidth()) *
                                 (imgWidthClipped / imgWidth)
                           : static_cast<float>(texture->second.getWidth()),
                       fittingMode == GUIFittingMode::CLIP
                           ? static_cast<float>(texture->second.getHeight()) *
                                 (imgHeightClipped / imgHeight)
                           : static_cast<float>(texture->second.getHeight())};

  if (imgPosX + imgWidthClipped <= posX + width &&
      imgPosY + imgHeightClipped <= posY + height) {
    auto &renderer{renderContext.getRenderer()};
    texture->second.render(imgPosX, imgPosY, &rect, imgWidthClipped,
                           imgHeightClipped, renderer);
  }
}
