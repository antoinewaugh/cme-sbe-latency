#pragma once

#include "Decoder.h"
#include "Handler.h"
#include "Receiver.h"

#include "sbe/MDIncrementalRefreshBook32.h"
#include "sbe/MDIncrementalRefreshDailyStatistics33.h"
#include "sbe/MDIncrementalRefreshLimitsBanding34.h"
#include "sbe/MDIncrementalRefreshOrderBook43.h"
#include "sbe/MDIncrementalRefreshSessionStatistics35.h"
#include "sbe/MDIncrementalRefreshTrade36.h"
#include "sbe/MDIncrementalRefreshTradeSummary42.h"
#include "sbe/MDIncrementalRefreshVolume37.h"
#include "sbe/MessageHeader.h"
#include "sbe/SnapshotFullRefresh38.h"
#include "sbe/SnapshotFullRefreshOrderBook44.h"

class SymbolFeed {

  bool recovery_mode = false;
  int seqnum = 0;

  void StartRecovery();
  void StopRecovery();

  Handler *handler_;
  Decoder *decoder_;

  OrderBook book_;

  Receiver incrementalA_;
  Receiver incrementalB_;
  Receiver snapshotA_;
  Receiver snapshotB_;

public:
  SymbolFeed(Handler *, Decoder *);
  ~SymbolFeed() = default;
  void OnMDIncrementalRefreshBook32(MDIncrementalRefreshBook32 &);
  void OnMDIncrementalRefreshDailyStatistics33(
      MDIncrementalRefreshDailyStatistics33 &);
  void
  OnMDIncrementalRefreshLimitsBanding34(MDIncrementalRefreshLimitsBanding34 &);
  void OnMDIncrementalRefreshSessionStatistics35(
      MDIncrementalRefreshSessionStatistics35 &);
  void OnMDIncrementalRefreshTrade36(MDIncrementalRefreshTrade36 &);
  void OnMDIncrementalRefreshVolume37(MDIncrementalRefreshVolume37 &);
  void
  OnMDIncrementalRefreshTradeSummary42(MDIncrementalRefreshTradeSummary42 &);
  void OnMDIncrementalRefreshOrderBook43(MDIncrementalRefreshOrderBook43 &);
  void OnMDSnapshotFullRefresh38(SnapshotFullRefresh38 &);
  void OnMDSnapshotFullRefreshOrderBook44(SnapshotFullRefreshOrderBook44 &);
};
