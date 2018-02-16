#pragma once

#include "MarketDataListener.h"
#include <vector>

namespace sp {
namespace lltp {
namespace cme {

struct Channel {
  virtual void SubscribeToSnapshotsForInstrument(std::uint32_t) = 0;
  virtual void UnsubscribeToSnapshotsForInstrument(std::uint32_t) = 0;
  virtual void SubscribeInstrumentDownload() = 0;
  virtual void UnsubscribeInstrumentDownload() = 0;
  virtual void RegisterMarketDataListener(MarketDataListener* listener) = 0;
  virtual void RemoveMarketDataListener(MarketDataListener* listener) = 0;
  virtual std::vector<MarketDataListener*> GetMarketDataListeners() = 0;
};

}}}
