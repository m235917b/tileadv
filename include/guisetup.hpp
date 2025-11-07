#include "api.hpp"

class GameAPI;

struct Focussed {
  std::string id;
};

struct Vitals {
  int health;
};

void setupGUI(GameAPI &xtxApi);
