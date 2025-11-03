#pragma once

#include <string>

class ECSContext;

struct PlayerIDResource {
  std::string id;
};

void initPlayerIDResource(ECSContext &ecsContext);
