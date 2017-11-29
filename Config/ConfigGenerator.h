#pragma once

#include <vector>

#include "Config.h"

struct ConfigGenerator {
  ConfigGenerator(std::string filepath);
  std::vector<Config> Get();
};
