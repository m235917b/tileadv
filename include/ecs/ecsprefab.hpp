#pragma once

class ECSContext;

class ECSPrefab {
public:
  ECSPrefab();
  ~ECSPrefab() = default;

private:
  ECSContext &context;
};
