#pragma once

class ECSContext;

struct KeyMapResource {
  bool w;
  bool a;
  bool s;
  bool d;
};

void initKeyMapResource(ECSContext &ecsContext);
