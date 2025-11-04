#pragma once

class ECSContext;

struct CameraResource {
  int posX;
  int posY;
};

void initCameraResource(ECSContext &ecsContext);
