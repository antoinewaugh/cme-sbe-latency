#pragma once

#include "Connection.h"
#include "MulticastReceiver.h"

#include <functional>

struct DataSource {
  MulticastReceiver primary, secondary;
  DataSource(MulticastReceiver primary,
             MulticastReceiver secondary);
  void Join();
  void Leave();
};
