#pragma once

#include "ChannelAccessor.h"

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

class InstrumentMdHandler {

public:

  void OnSnapshot(SnapshotFullRefresh38&);
  void OnIncrementalRefreshBookEntry(MDIncrementalRefreshBook32::NoMDEntries &);
  void OnIncrementalRefreshTradeEntry(MDIncrementalRefreshTrade36::NoMDEntries&);
  void OnIncrementalRefreshVolume(MDIncrementalRefreshVolume37&);
  void OnIncrementalTradeSumary(MDIncrementalRefreshTradeSummary42& );
  void OnIncrementalDailyStatistics(MDIncrementalRefreshDailyStatistics33&);
  void OnIncrementalSessionStatistics(MDIncrementalRefreshSessionStatistics35&);
  void OnIncrementalLimitsBanding(MDIncrementalRefreshLimitsBanding34&);
  void OnSecurityStatus(MDIncrementalRefreshLimitsBanding34&);
  void OnChannelReset();
  void Reset();
  InstrumentMdHandler(ChannelAccessor *);

};
