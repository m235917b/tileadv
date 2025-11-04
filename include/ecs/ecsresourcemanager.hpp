#pragma once

#include <any>
#include <typeindex>
#include <unordered_map>

class ECSResourceManager {
public:
  ECSResourceManager() = default;
  ~ECSResourceManager() = default;

  void setResource(std::type_index type, std::any resource);
  std::any *getResource(const std::type_index &type);
  const std::any *getResource(const std::type_index &type) const;

  template <typename ResourceType> void setResource(ResourceType resource) {
    setResource(std::type_index(typeid(ResourceType)),
                std::make_any<ResourceType>(resource));
  }

  template <typename ResourceType> const ResourceType *getResource() const {
    return std::any_cast<ResourceType>(
        getResource(std::type_index(typeid(ResourceType))));
  }

  template <typename ResourceType> ResourceType *getResource() {
    return std::any_cast<ResourceType>(
        getResource(std::type_index(typeid(ResourceType))));
  }

private:
  std::unordered_map<std::type_index, std::any> resources;
};
