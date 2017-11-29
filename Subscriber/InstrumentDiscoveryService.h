#pragma once

#include "ConfigGenerator.h"

/**
 *
 * Questions:
 *  - Decoder - will it be shared or separate? can we create one per channel to
 * be used? this could cause problems if threading is used
 *            - certainly the decoder should be used to decode as its required
 * for other subscriptoins
 *
 *  - Discovery() should occur on construction, need to signal when ready
 *      - look into async with block on another thread?
 *
 *      auto result = InstrumentDiscovery(config).fetch()
 *
 */

struct InstrumentDiscoveryService {
  InstrumentDiscoveryService(ConfigGenerator config);

private:
  ConfigGenerator config_;
};
