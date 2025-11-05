#include <functional>
#include <typeindex>
#include <vector>

#include "ecs/ecsstore.hpp"

void ECSStore::destroyEntity(const std::string &entityId) {
  for (auto &pool : componentStores) {
    pool.second.erase(entityId);
  }
}

void ECSStore::upsertComponent(std::string entityId, std::any component) {
  auto &pool{componentStores[std::type_index(component.type())]};
  pool[std::move(entityId)] = std::move(component);
}

void ECSStore::updateComponent(std::type_index type,
                               const std::string &entityId,
                               UpdateFnAny update) {
  const auto &it{componentStores.find(type)};

  if (it == componentStores.end()) {
    return;
  }

  const auto &ent{it->second.find(entityId)};

  if (ent == it->second.end()) {
    return;
  }

  ent->second = update(ent->second);
}

std::any *ECSStore::getComponent(const std::string &entityId,
                                 const std::type_index &type) {
  const auto it{componentStores.find(type)};

  if (it != componentStores.end()) {
    auto &pool{it->second};
    auto compIt{pool.find(entityId)};

    if (compIt != pool.end()) {
      return &(compIt->second);
    }
  }

  return nullptr;
}

const std::any *ECSStore::getComponent(const std::string &entityId,
                                       const std::type_index &type) const {
  return const_cast<ECSStore *>(this)->getComponent(entityId, type);
}

void ECSStore::view(
    const std::vector<std::type_index> &types,
    const std::function<void(const std::string &,
                             const std::vector<const std::any *> &)> &f) const {
  if (types.size() == 0) {
    return;
  }

  const auto smallest{getSmallestContainer(types)};

  if (!smallest) {
    return;
  }

  std::vector<const std::any *> components{};
  components.reserve(types.size());

  for (const auto &[id, _] : *smallest) {
    bool ok{true};

    components.clear();

    for (const auto &t : types) {
      const auto component{getComponent(id, t)};

      if (!component) {
        ok = false;
        break;
      }

      components.emplace_back(component);
    }

    if (ok) {
      f(id, components);
    }
  }
}

const std::unordered_map<std::string, std::any> *ECSStore::getSmallestContainer(
    const std::vector<std::type_index> &types) const {
  const std::unordered_map<std::string, std::any> *smallest{nullptr};
  for (const auto &type : types) {
    const auto it{componentStores.find(type)};

    if (it == componentStores.end()) {
      return nullptr;
    }

    if (!smallest || smallest->size() > it->second.size()) {
      smallest = &(it->second);
    }
  }

  return smallest;
}

bool ECSStore::hasComponent(std::type_index type, const std::string &id) const {
  const auto &it{componentStores.find(type)};

  if (it == componentStores.end()) {
    return false;
  }

  const auto &entity{it->second.find(id)};

  if (entity == it->second.end()) {
    return false;
  }

  return true;
}
