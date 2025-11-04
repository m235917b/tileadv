#pragma once

#include <string>

class ECSContext;

std::string registerMovementSystem(const std::string &phase,
                                   ECSContext &ecsContext);
