#pragma once

#include "Packet.h"
#include "Message.h"
#include "InstrumentController.h"

#include "sbe/MDIncrementalRefreshBook32.h"
#include "sbe/MDIncrementalRefreshDailyStatistics33.h"
#include "sbe/MDIncrementalRefreshLimitsBanding34.h"
#include "sbe/MDIncrementalRefreshSessionStatistics35.h"
#include "sbe/MDIncrementalRefreshTrade36.h"
#include "sbe/MDIncrementalRefreshTradeSummary42.h"
#include "sbe/MDIncrementalRefreshVolume37.h"
#include "sbe/QuoteRequest39.h"
#include "sbe/SecurityStatus30.h"
#include "sbe/SnapshotFullRefresh38.h"
#include <map>

class ChannelController {

public:
  ChannelController();
  void OnIncrementalPacket(Packet*);
  void OnSnapshotPacket(Packet*);

private:
  void OnIncrementalMessage(Message&);
  void HandleSnapshotMessage(Message& m);
  template<typename T> void HandleIncrementalMessage(Message&);
  InstrumentController* GetInstrumentController(uint32_t securityid);
  std::map<uint32_t, InstrumentController> instrument_controllers_;
};


