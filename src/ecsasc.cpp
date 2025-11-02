#include "ecsasc.hpp"

ECSASC::ECSASC()
    : ecsContext(), ecsApi(ecsContext), renderContext(),
      renderContextWrapper(renderContext), guiContext(renderContextWrapper) {};

void ECSASC::init() { ecsview::init(renderContext); }

void ECSASC::run() {
  init();

  SDL_Delay(2000);

  destroy();
}

void ECSASC::destroy() { ecsview::destroy(renderContext); }
