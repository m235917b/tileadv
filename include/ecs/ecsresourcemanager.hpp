#pragma once

#include <any>
#include <typeindex>
#include <unordered_map>

class ECSResourceManager {
public:
  ECSResourceManager() {}
  ~ECSResourceManager() = default;

  void setResource(std::type_index type, std::any resource) {
    resources[type] = std::move(resource);
  }

  const std::any *getResource(const std::type_index &type) const {
    const auto it{resources.find(type)};

    if (it == resources.end()) {
      return nullptr;
    }

    return &(it->second);
  }

  template <typename ResourceType> void setResource(ResourceType resource) {
    setResource(std::type_index(typeid(ResourceType)),
                std::make_any<ResourceType>(resource));
  }

  template <typename ResourceType> const ResourceType *getResource() const {
    return std::any_cast<ResourceType>(
        getResource(std::type_index(typeid(ResourceType))));
  }

private:
  std::unordered_map<std::type_index, std::any> resources;
};
