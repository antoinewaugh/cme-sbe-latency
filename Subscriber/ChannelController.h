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
#include <chrono>
#include <iostream>

class ChannelController {

public:
  ChannelController() = default;
  ChannelController(ChannelAccessor* channel);
  void OnIncrementalPacket(Packet*);
  void OnSnapshotPacket(Packet*);
  void AddOutOfSyncInstrument(uint32_t securityid);
  bool RemoveOutOfSyncInstrument(uint32_t securityid);
  bool HasOutOfSyncInstruments();
  void Subscribe(uint32_t securityid);
  void Unsubscribe(uint32_t securityid);

private:
  template<typename T>
  void HandleIncrementalMessage(Message& m) {

    auto exch_time = refresh.transactTime();
    unsigned long ns_timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch())
        .count();

    auto delay_ns = ns_timestamp - exch_time;
    auto delay_ms = delay_ns / 1000000.0;
    std::cout << "Delay: " << delay_ms << "ms" <<'\n';

    auto message = m.Get<T>();
    auto& entry = message.noMDEntries();
    while(entry.hasNext()) {
      entry.next();
      auto inst_controller = GetInstrumentController(entry.securityID());
      if(inst_controller) {
        inst_controller->OnIncremental(entry);
      }
    }
  }
  void OnIncrementalMessage(Message&);
  void HandleSnapshotMessage(Message& m);
//  template<typename T> void HandleIncrementalMessage(Message&);
  void HandleIncrementalSecurityStatus(Message& m);
  void HandleIncrementalQuoteRequest(Message& m);
  InstrumentController* GetInstrumentController(uint32_t securityid);
  std::map<uint32_t, InstrumentController> instrument_controllers_;
  std::vector<uint32_t> outofsync_instruments_;
  ChannelAccessor* channel_ = nullptr;
};



