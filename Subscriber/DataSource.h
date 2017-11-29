#pragma once

#include "Connection.h"
#include "MulticastReceiver.h"

#include <functional>

struct DataSource {
  std::unique_ptr<MulticastReceiver> primary, secondary;
  DataSource(std::unique_ptr<MulticastReceiver> primary,
             std::unique_ptr<MulticastReceiver> secondary);
  void Join();
  void Leave();
};
