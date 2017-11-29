#pragma once

#include "Connection.h"
#include "MulticastReceiver.h"

#include <functional>
#include <ostream>

struct DataSource {
  MulticastReceiver primary, secondary;
  DataSource(MulticastReceiver primary,
             MulticastReceiver secondary);
  void Join();
  void Leave();

  friend std::ostream &operator<<(std::ostream &os, const DataSource &source);
};
