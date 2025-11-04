#include "ecs/ecsresourcemanager.hpp"

void ECSResourceManager::setResource(std::type_index type, std::any resource) {
  resources[type] = std::move(resource);
}

std::any *ECSResourceManager::getResource(const std::type_index &type) {
  const auto it{resources.find(type)};

  if (it == resources.end()) {
    return nullptr;
  }

  return &(it->second);
}

const std::any *
ECSResourceManager::getResource(const std::type_index &type) const {
  const auto it{resources.find(type)};

  if (it == resources.end()) {
    return nullptr;
  }

  return &(it->second);
}
