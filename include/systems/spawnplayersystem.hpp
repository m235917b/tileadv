#pragma once

#include <string>

class ECSAPI;
class ECSContext;

void registerSpawnPlayerSystem(const std::string &phase, ECSContext &ecsContext,
                               ECSAPI &ecsApi);
