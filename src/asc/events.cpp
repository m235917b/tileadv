#include "asc/events.hpp"

#include <any>
#include <typeindex>

#include "asc/events.hpp"
#include "asc/resources.hpp"
#include "ecs/ecs.hpp"
#include "gui/guicontext.hpp"

void subscribeKeyDownEventListener(ECSContext &context) {
  context.getEventBus().subscribe<GameKeyDownEvent>(
      [](ECSContext &context, const GameKeyDownEvent &event) {
        auto keyMap{
            *context.getResourceManager().getResource<KeyMapResource>()};

        if (event.keycode == SDLK_W) {
          keyMap.w = true;
        }
        if (event.keycode == SDLK_A) {
          keyMap.a = true;
        }
        if (event.keycode == SDLK_S) {
          keyMap.s = true;
        }
        if (event.keycode == SDLK_D) {
          keyMap.d = true;
        }

        context.getCommandBuffer().upsertResource(keyMap);
      });
}

void subscribeKeyUpEventListener(ECSContext &context) {
  context.getEventBus().subscribe<GameKeyUpEvent>(
      [](ECSContext &context, const GameKeyUpEvent &event) {
        auto keyMap{
            *context.getResourceManager().getResource<KeyMapResource>()};

        if (event.keycode == SDLK_W) {
          keyMap.w = false;
        }
        if (event.keycode == SDLK_A) {
          keyMap.a = false;
        }
        if (event.keycode == SDLK_S) {
          keyMap.s = false;
        }
        if (event.keycode == SDLK_D) {
          keyMap.d = false;
        }

        context.getCommandBuffer().upsertResource(keyMap);
      });
}

void subscribeGUIMouseMotionListener(ECSContext &ecsContext) {
  ecsContext.getEventBus().subscribe(
      std::type_index(typeid(MouseMotionEvent)),
      [](ECSContext &context, const auto &eventAny) {
        const auto &event{std::any_cast<MouseMotionEvent>(eventAny)};
        auto guiContext{std::any_cast<GUIContextResource>(
                            context.getResourceManager().getResource(
                                std::type_index(typeid(GUIContextResource))))
                            ->guiContext};

        guiContext->mouseMotionListener(event.posX, event.posY);
      });
}

void subscribeGUIKeyDownEventListener(ECSContext &ecsContext) {
  ecsContext.getEventBus().subscribe(
      std::type_index(typeid(KeyDownEvent)),
      [](ECSContext &context, const auto &eventAny) {
        const auto &event{std::any_cast<KeyDownEvent>(eventAny)};
        auto guiContext{std::any_cast<GUIContextResource>(
                            context.getResourceManager().getResource(
                                std::type_index(typeid(GUIContextResource))))
                            ->guiContext};

        guiContext->keyDownListener(event.keycode);
      });
}

void subscribeGUIMouseDownEventListener(ECSContext &ecsContext) {
  ecsContext.getEventBus().subscribe(
      std::type_index(typeid(MouseDownEvent)),
      [](ECSContext &context, const auto &eventAny) {
        const auto &event{std::any_cast<MouseDownEvent>(eventAny)};
        auto guiContext{std::any_cast<GUIContextResource>(
                            context.getResourceManager().getResource(
                                std::type_index(typeid(GUIContextResource))))
                            ->guiContext};

        guiContext->mouseButtonDownListener(event.button);
      });
}

void initASCEvents(ECSContext &ecsContext) {
  subscribeKeyDownEventListener(ecsContext);
  subscribeKeyUpEventListener(ecsContext);
  subscribeGUIMouseMotionListener(ecsContext);
  subscribeGUIKeyDownEventListener(ecsContext);
  subscribeGUIMouseDownEventListener(ecsContext);
}
