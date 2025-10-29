#pragma once

#include <any>
#include <string>
#include <typeindex>
#include <unordered_map>

class ECSStore {
public:
  ECSStore() = default;
  ~ECSStore() = default;

  void createEntity([[maybe_unused]] const std::string &entityId) {
    // Entities are implicitly created when components are added.
  }

  void destroyEntity(const std::string &entityId) {
    for (auto &pool : componentStores) {
      pool.second.erase(entityId);
    }
  }

  template <typename T>
  void addComponent(const std::string &entityId, T component) {
    auto &pool = componentStores[std::type_index(typeid(T))];
    pool[entityId] = std::make_any<T>(std::move(component));
  }

  template <typename T> T *getComponent(const std::string &entityId) {
    auto it = componentStores.find(std::type_index(typeid(T)));

    if (it != componentStores.end()) {
      auto &pool = it->second;
      auto compIt = pool.find(entityId);

      if (compIt != pool.end()) {
        return std::any_cast<T>(&(compIt->second));
      }
    }

    return nullptr;
  }

  template <typename... Ts, typename Func> void view(Func f) {
    auto &firstPool =
        componentStores[typeid(std::tuple_element_t<0, std::tuple<Ts...>>)];

    for (auto &[id, anyVal] : firstPool) {
      if ((getComponent<Ts>(id) && ...)) {
        f(id, *getComponent<Ts>(id)...);
      }
    }
  }

private:
  std::unordered_map<std::type_index, std::unordered_map<std::string, std::any>>
      componentStores;
};
