#pragma once

#include <DepthBook.h>
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
  void OnIncremental(MDIncrementalRefreshBook32::NoMDEntries &);
  void OnIncremental(MDIncrementalRefreshTrade36::NoMDEntries&);
  void OnIncremental(MDIncrementalRefreshVolume37::NoMDEntries&);
  void OnIncremental(MDIncrementalRefreshTradeSummary42::NoMDEntries& );
  void OnIncremental(MDIncrementalRefreshDailyStatistics33::NoMDEntries&);
  void OnIncremental(MDIncrementalRefreshSessionStatistics35::NoMDEntries&);
  void OnIncremental(MDIncrementalRefreshLimitsBanding34::NoMDEntries&);
  void OnSecurityStatus(SecurityStatus30&);
  void OnChannelReset();
  void Reset();

private:
  DepthBook book_;
  DepthBook implbook_;

  void ClearState();
};
