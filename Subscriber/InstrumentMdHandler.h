#pragma once

#include <DepthBook.h>
#include "ChannelAccessor.h"
#include "Handler.h"

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
#include "SessionStatistics.h"
#include "Instrument.h"

namespace sp {
namespace lltp {
namespace cme {

  class InstrumentMdHandler {

  public:
    InstrumentMdHandler(Instrument);

    void OnSnapshot(SnapshotFullRefresh38 &, std::uint64_t);

    void OnIncremental(MDIncrementalRefreshBook32::NoMDEntries &, std::uint64_t);

    void OnIncremental(MDIncrementalRefreshTrade36::NoMDEntries &, std::uint64_t);

    void OnIncremental(MDIncrementalRefreshVolume37::NoMDEntries &, std::uint64_t);

    void OnIncremental(MDIncrementalRefreshTradeSummary42::NoMDEntries &, std::uint64_t);

    void OnIncremental(MDIncrementalRefreshDailyStatistics33::NoMDEntries &, std::uint64_t);

    void OnIncremental(MDIncrementalRefreshSessionStatistics35::NoMDEntries &, std::uint64_t);

    void OnIncremental(MDIncrementalRefreshLimitsBanding34::NoMDEntries &, std::uint64_t);

    void OnSecurityStatus(SecurityStatus30 &);

    void OnChannelReset();

    void Reset();

    void Commit();

  private:
    Instrument instrument_;
    DepthBook book_;
    DepthBook implbook_;
    SessionStatistics statistics_;
    bool statechange_;
    Handler handler_;

    void ClearState();
  };

}}} // namespace
