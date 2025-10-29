#include "ecs/ecscontext.hpp"

ECSContext::ECSContext() : store(), scheduler(*this) {}

ECSStore &ECSContext::getStore() { return store; }

ECSScheduler &ECSContext::getScheduler() { return scheduler; }
