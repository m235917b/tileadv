#pragma once

#include <algorithm>
#include <any>
#include <string>
#include <tuple>
#include <typeindex>
#include <unordered_map>
#include <utility>

class ECSStore {
public:
  ECSStore() = default;
  ~ECSStore() = default;

  void destroyEntity(const std::string &entityId) {
    for (auto &pool : componentStores) {
      pool.second.erase(entityId);
    }
  }

  template <typename T>
  void upsertComponent(const std::string &entityId, T component) {
    auto &pool = componentStores[std::type_index(typeid(T))];
    pool[entityId] = std::make_any<T>(std::move(component));
  }

  void upsertComponent(const std::string &entityId, std::any component) {
    auto &pool = componentStores[std::type_index(component.type())];
    pool[entityId] = std::move(component);
  }

  template <typename T>
  const T *getComponent(const std::string &entityId) const {
    auto it = componentStores.find(std::type_index(typeid(T)));

    if (it != componentStores.end()) {
      const auto &pool = it->second;
      const auto compIt = pool.find(entityId);

      if (compIt != pool.end()) {
        return std::any_cast<T>(&(compIt->second));
      }
    }

    return nullptr;
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

  std::any *getComponent(const std::string &entityId,
                         const std::type_index &type) {
    auto it = componentStores.find(type);

    if (it != componentStores.end()) {
      auto &pool = it->second;
      auto compIt = pool.find(entityId);

      if (compIt != pool.end()) {
        return &(compIt->second);
      }
    }

    return nullptr;
  }

  const std::any *getComponent(const std::string &entityId,
                               const std::type_index &type) const {
    auto it = componentStores.find(type);

    if (it != componentStores.end()) {
      auto &pool = it->second;
      auto compIt = pool.find(entityId);

      if (compIt != pool.end()) {
        return &(compIt->second);
      }
    }

    return nullptr;
  }

  template <typename... Ts, typename Func> void view(Func &&f) {
    using First = std::tuple_element_t<0, std::tuple<Ts...>>;
    auto it = componentStores.find(std::type_index(typeid(First)));
    if (it == componentStores.end()) {
      return;
    }

    auto &firstPool = it->second;
    for (auto &[id, anyVal] : firstPool) {
      if ((getComponent<Ts>(id) && ...)) {
        f(id, (*getComponent<Ts>(id))...);
      }
    }
  }

  void
  view(std::vector<std::type_index> types,
       std::function<void(const std::string &, std::vector<std::any *> &)> f) {
    auto it{componentStores.find(types.at(0))};

    if (it == componentStores.end()) {
      return;
    }

    auto &firstPool{it->second};

    for (auto &[id, anyVal] : firstPool) {
      std::vector<std::any *> components{};
      bool ok{true};

      for (const auto &t : types) {
        auto component{getComponent(id, t)};

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

private:
  std::unordered_map<std::type_index, std::unordered_map<std::string, std::any>>
      componentStores;
};
