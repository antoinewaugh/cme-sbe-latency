#pragma once

#include "ConfigGenerator.h"
#include "Decoder.h"

struct Instrument {};

struct InstrumentDiscoveryService {
  InstrumentDiscoveryService(ConfigGenerator config);

private:
  Decoder decoder_;
  DataSource instrument_;

  void OnInstrument();
};
