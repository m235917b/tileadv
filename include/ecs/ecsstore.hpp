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

  void destroyEntity(const std::string &entityId);
  void upsertComponent(const std::string &entityId, std::any component);
  std::any *getComponent(const std::string &entityId,
                         const std::type_index &type);
  const std::any *getComponent(const std::string &entityId,
                               const std::type_index &type) const;
  void view(const std::vector<std::type_index> &types,
            const std::function<void(const std::string &,
                                     const std::vector<const std::any *> &)> &f)
      const;

  template <typename T>
  void upsertComponent(const std::string &entityId, T component) {
    upsertComponent(entityId, std::move(std::make_any<T>(component)));
  }

  template <typename T>
  const T *getComponent(const std::string &entityId) const {
    return getComponent(entityId, std::type_index(typeid(T)));
  }

  template <typename T> T *getComponent(const std::string &entityId) {
    return getComponent(entityId, std::type_index(typeid(T)));
  }

  template <typename... Ts, typename Func> void view(Func &&f) const {
    static_assert(sizeof...(Ts) > 0);

    std::vector<std::type_index> types;
    types.reserve(sizeof...(Ts));
    (types.emplace_back(std::type_index(typeid(std::decay_t<Ts>))), ...);

    const auto wrap{[f = std::forward<Func>(f)](
                        const std::string &entityId,
                        const std::vector<const std::any *> &components) {
      auto apply{[&]<std::size_t... Is>(std::index_sequence<Is...>) {
        f(entityId, (std::any_cast<const Ts &>(*components[Is]))...);
      }};
      apply(std::make_index_sequence<sizeof...(Ts)>{});
    }};

    view(types, wrap);
  }

private:
  std::unordered_map<std::type_index, std::unordered_map<std::string, std::any>>
      componentStores;

  const std::unordered_map<std::string, std::any> *
  getSmallestContainer(const std::vector<std::type_index> &types) const;

  template <typename... Ts>
  const std::unordered_map<std::string, std::any> *
  getSmallestContainer() const {
    std::vector<std::type_index> types;
    types.reserve(sizeof...(Ts));
    (types.emplace_back(std::type_index(typeid(std::decay_t<Ts>))), ...);
    return getSmallestContainer(types);
  }
};
