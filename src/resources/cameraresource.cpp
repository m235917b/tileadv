#include "resources/cameraresource.hpp"

#include "ecs/ecs.hpp"

void initCameraResource(ECSContext &ecsContext) {
  ecsContext.getCommandBuffer().upsertResource(CameraResource{0, 0});
}
