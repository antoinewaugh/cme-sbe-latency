#pragma once

#include "sbe/MessageHeader.h"

#include "sbe/AdminHeartbeat12.h"
#include "sbe/ChannelReset4.h"
#include "sbe/MDIncrementalRefreshBook32.h"
#include "sbe/MDIncrementalRefreshDailyStatistics33.h"
#include "sbe/MDIncrementalRefreshLimitsBanding34.h"
#include "sbe/MDIncrementalRefreshOrderBook43.h"
#include "sbe/MDIncrementalRefreshSessionStatistics35.h"
#include "sbe/MDIncrementalRefreshTrade36.h"
#include "sbe/MDIncrementalRefreshTradeSummary42.h"
#include "sbe/MDIncrementalRefreshVolume37.h"
#include "sbe/MDInstrumentDefinitionFuture27.h"
#include "sbe/MDInstrumentDefinitionOption41.h"
#include "sbe/MDInstrumentDefinitionSpread29.h"
#include "sbe/MessageHeader.h"
#include "sbe/QuoteRequest39.h"
#include "sbe/SecurityStatus30.h"
#include "sbe/SnapshotFullRefresh38.h"
#include "sbe/SnapshotFullRefreshOrderBook44.h"

class Message {
public:
  uint16_t GetTemplateId();
  template<typename T> T Get();

private:

  char *buffer;
  uint16_t offset;
  uint16_t block_length;
  uint16_t version;
  uint16_t buffer_length;

  // reuses objects to prevent allocations
  MessageHeader header;
  ChannelReset4 channel_reset;
  AdminHeartbeat12 admin_heartbeat;
  MDInstrumentDefinitionFuture27 definition_future;
  MDInstrumentDefinitionOption41 definition_option;
  MDInstrumentDefinitionSpread29 definition_spread;
  QuoteRequest39 quote_request;
  SecurityStatus30 security_status;
  SnapshotFullRefresh38 snapshot_full;
  SnapshotFullRefreshOrderBook44 snapshot_full_order_book;
  MDIncrementalRefreshBook32 incremental_refresh_book;
  MDIncrementalRefreshVolume37 incremental_volume;
  MDIncrementalRefreshTrade36 incremental_trade;
  MDIncrementalRefreshTradeSummary42 incremental_trade_summary;
  MDIncrementalRefreshOrderBook43 incremental_order_book;
  MDIncrementalRefreshSessionStatistics35 session_statistics;
  MDIncrementalRefreshLimitsBanding34 limit_banding;
  MDIncrementalRefreshDailyStatistics33 daily_statistics;

  // wrapper functions
  ChannelReset4& GetChannelReset();
  AdminHeartbeat12& GetAdminHeartbeat();
  MDInstrumentDefinitionFuture27& GetInstrumentDefinitionFuture();
  MDInstrumentDefinitionOption41& GetInstrumentDefinitionOption();
  MDInstrumentDefinitionSpread29& GetInstrumentDefinitionSpread();
  QuoteRequest39& GetQuoteRequest();
  SecurityStatus30& GetSecurityStatus();
  SnapshotFullRefresh38& GetSnapshotFullRefresh();
  SnapshotFullRefreshOrderBook44& GetSnapshotFullRefreshOrderBook();
  MDIncrementalRefreshBook32& GetIncrementalRefreshBook();
  MDIncrementalRefreshVolume37& GetIncrementalRefreshVolume();
  MDIncrementalRefreshTrade36& GetIncrementalRefreshTrade();
  MDIncrementalRefreshOrderBook43& GetIncrementalRefreshOrderBook();
  MDIncrementalRefreshSessionStatistics35& GetIncrementalRefreshSessionStatistics();
  MDIncrementalRefreshLimitsBanding34& GetIncrementalRefreshLimitsBanding();
  MDIncrementalRefreshDailyStatistics33& GetIncrementalRefreshDailyStatistics();

};


