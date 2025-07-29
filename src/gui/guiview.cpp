#include <cmath>

#include "gui/asciiatlas.hpp"
#include "gui/guicomponent.hpp"
#include "gui/guitreewalker.hpp"
#include "gui/guiview.hpp"
#include "utils/rendercontext.hpp"

GUIView::GUIView(const RenderContext &renderContext)
    : renderContext(renderContext), asciiGrey(), layoutBuffer() {}

void GUIView::drawGUI(const std::string &selected) {
  auto &renderer{renderContext.getRenderer()};

  layoutBuffer.forEach([&selected, &renderer, this](GUILayoutData data) {
    const auto component = data.component;
    const auto rect = data.layout;

    if (component->getBorder()) {
      SDL_SetRenderDrawColor(&renderer, 0x60, 0x60, 0x60, 0xFF);
      SDL_RenderRect(&renderer, &rect);
    }

    if (component->getBackground()) {
      SDL_SetRenderDrawColor(&renderer, 0x60, 0x60, 0x60, 0xFF);
      SDL_RenderFillRect(&renderer, &rect);
    }

    if (!component->getText().empty()) {
      const auto text{(selected == component->getId() ? ">" : " ") +
                      component->getText() + " "};
      drawText(rect, text, component->getScale(), component->getFittingMode(),
               component->isCenteredLeft(), component->isCenteredTop());
    } else if (selected == component->getId()) {
      SDL_SetRenderDrawColor(&renderer, 0xFF, 0x40, 0x40, 0x20);
      SDL_RenderFillRect(&renderer, &rect);
    }

    if (!component->getImage().empty()) {
      drawImage(rect, component->getImage(), component->getScale(),
                component->getFittingMode(), component->isCenteredLeft(),
                component->isCenteredTop());
    }
  });
}

void GUIView::drawText(const SDL_FRect rect, const std::string &text,
                       const float size, GUIFittingMode fittingMode,
                       const bool centerLeft, const bool centerTop) {
  auto &renderer{renderContext.getRenderer()};

  const auto charWidth{fittingMode == GUIFittingMode::SCALE
                           ? rect.w / static_cast<float>(text.length())
                           : asciiWidth * size};
  const auto charHeight{asciiHeight * size};
  const auto leftMargin{
      centerTop && fittingMode != GUIFittingMode::SCALE
          ? std::max(0.f, (rect.w - text.length() * charWidth) / 2.f)
          : 0.f};
  const auto topMargin{centerLeft && fittingMode != GUIFittingMode::SCALE
                           ? std::max(0.f, (rect.h - charHeight) / 2.f)
                           : 0.f};
  const auto textPosY{rect.y + topMargin};
  const auto charHeightClipped{
      fittingMode == GUIFittingMode::CLIP
          ? std::max(0.f, std::min(charHeight, rect.y + rect.h - textPosY))
          : (fittingMode == GUIFittingMode::SCALE ? rect.h : charHeight)};

  for (size_t i = 0; i < text.length(); ++i) {
    auto spriteCoords{AsciiAtlas::getSpriteCoords(text[i])};
    const auto textPosX{rect.x + static_cast<float>(i) * charWidth +
                        leftMargin};

    const auto charWidthClipped{
        fittingMode == GUIFittingMode::CLIP
            ? std::max(0.f, std::min(charWidth, rect.x + rect.w - textPosX))
            : charWidth};

    if (fittingMode == GUIFittingMode::CLIP) {
      spriteCoords.w = spriteCoords.w * (charWidthClipped / charWidth);
      spriteCoords.h = spriteCoords.h * (charHeightClipped / charHeight);
    }

    if (textPosX + charWidthClipped <= rect.x + rect.w &&
        textPosY + charHeightClipped <= rect.y + rect.h) {
      SDL_SetRenderDrawColor(&renderer, 0x50, 0x50, 0x50, 0xFF);
      asciiGrey.render(textPosX, textPosY, &spriteCoords, charWidthClipped,
                       charHeightClipped, renderer);
    }
  }
}

void GUIView::drawImage(const SDL_FRect rect, const std::string &path,
                        const float size, GUIFittingMode fittingMode,
                        const bool centerLeft, const bool centerTop) {
  const auto &texture{images.find(path)};

  if (texture == images.end()) {
    return;
  }

  const auto imgWidth{fittingMode == GUIFittingMode::SCALE
                          ? rect.w
                          : static_cast<float>(texture->second.getWidth()) *
                                size};
  const auto imgHeight{fittingMode == GUIFittingMode::SCALE
                           ? rect.h
                           : static_cast<float>(texture->second.getHeight()) *
                                 size};
  const auto imgPosX{
      centerTop ? rect.x + std::max(0.f, (rect.w - imgWidth) / 2.f) : rect.x};
  const auto imgPosY{
      centerLeft ? rect.y + std::max(0.f, (rect.h - imgHeight) / 2.f) : rect.y};
  const auto imgWidthClipped{
      fittingMode == GUIFittingMode::CLIP
          ? std::max(0.f, std::min(imgWidth, rect.x + rect.w - imgPosX))
          : imgWidth};
  const auto imgHeightClipped{
      fittingMode == GUIFittingMode::CLIP
          ? std::max(0.f, std::min(imgHeight, rect.y + rect.h - imgPosY))
          : imgHeight};

  const SDL_FRect imgRect{
      0.f, 0.f,
      fittingMode == GUIFittingMode::CLIP
          ? static_cast<float>(texture->second.getWidth()) *
                (imgWidthClipped / imgWidth)
          : static_cast<float>(texture->second.getWidth()),
      fittingMode == GUIFittingMode::CLIP
          ? static_cast<float>(texture->second.getHeight()) *
                (imgHeightClipped / imgHeight)
          : static_cast<float>(texture->second.getHeight())};

  if (imgPosX + imgWidthClipped <= rect.x + rect.w &&
      imgPosY + imgHeightClipped <= rect.y + rect.h) {
    auto &renderer{renderContext.getRenderer()};
    texture->second.render(imgPosX, imgPosY, &imgRect, imgWidthClipped,
                           imgHeightClipped, renderer);
  }
}

void GUIView::allocateLayoutCache(const size_t size) {
  layoutBuffer.allocate(size);
}

void GUIView::recomputeLayoutCache(GUIComponent &component) {
  GUITreeWalker::traverse(
      component,
      [this](GUIComponent &node, float &posXParentAbs, float &posYParentAbs,
             float &widthParent, float &heightParent) {
        if (!node.isVisible()) {
          return false;
        }

        const auto screenWidth{renderContext.getScreenWidth()};
        const auto screenHeight{renderContext.getScreenHeight()};

        const SDL_FRect rect{
            posXParentAbs + node.getPosX() * widthParent * screenWidth,
            posYParentAbs + node.getPosY() * heightParent * screenHeight,
            node.getWidth() * widthParent * screenWidth,
            node.getHeight() * heightParent * screenHeight};

        layoutBuffer.push({&node, rect});

        posXParentAbs = rect.x;
        posYParentAbs = rect.y;
        widthParent = node.getWidth() * widthParent;
        heightParent = node.getHeight() * heightParent;

        return true;
      },
      0.f, 0.f, 1.f, 1.f);
}

GUIComponent *GUIView::getHitComponent(const float posX, const float posY) {
  auto stop{false};
  GUIComponent *component{nullptr};

  layoutBuffer.forEach(
      [&posX, &posY, &stop, &component](GUILayoutData &data) {
        if (posX >= data.layout.x && posX <= data.layout.x + data.layout.w &&
            posY >= data.layout.y && posY <= data.layout.y + data.layout.h) {
          component = data.component;
          stop = true;
        }
      },
      stop, true);

  return component;
}
