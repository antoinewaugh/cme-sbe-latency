#pragma once

#include "DepthBook.h"
#include "Handler.h"

#include <functional>

//#include "sbe/AdminLogin15.h"     // TCP/IP only
//#include "sbe/AdminLogout16.h"    // TCP/IP only
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

using namespace sbe;

constexpr int kMsgSize = 2;
constexpr int kByteOffest = 12;
constexpr int kMsgHeaderVersion = 0;

enum ChannelStatus { InstrumentsRecovered, MarketRecovered, TimeoutIdentified };

struct Decoder {

  struct Message {
    char *buffer;
    uint16_t offset;
    uint16_t block_length;
    uint16_t version;
    uint16_t buffer_length;
  };

  size_t DecodePacket(char *, size_t);
  Decoder(uint64_t securityid, const Handler &handler,
          std::function<void(ChannelStatus)> OnChannelStatus);
  static uint32_t GetSeqNum(char*, size_t);
  void Clear();

private:
  bool market_recovery_ = false;
  int rpt_seq_ = 0;

  uint64_t securityid_;
  uint64_t groupid_;    // filtering on groups required

  Handler handler_;
  DepthBook book_;

  std::function<void(ChannelStatus)> cb_channel_;

  void StartRecovery();
  void StopRecovery();

  template <class T> bool CheckStream(T);
  void ClearAllStates();

  void HandleBidEntry(MDUpdateAction::Value action, int level, float price,
                      int volume);
  void HandleAskEntry(MDUpdateAction::Value action, int level, float price,
                      int volume);

  bool DecodeChannelReset(ChannelReset4 &, Decoder::Message);

  bool DecodeAdminHeartbeat(AdminHeartbeat12 &, Decoder::Message);

  bool DecodeInstrumentDefinitionFuture(MDInstrumentDefinitionFuture27 &,
                                        Decoder::Message);

  bool DecodeInstrumentDefinitionOption(MDInstrumentDefinitionOption41 &,
                                        Decoder::Message);

  bool DecodeInstrumentDefinitionSpread(MDInstrumentDefinitionSpread29 &,
                                        Decoder::Message);

  bool DecodeQuoteRequest(QuoteRequest39 &, Decoder::Message);

  bool DecodeSecurityStatus(SecurityStatus30 &, Decoder::Message);

  bool DecodeIncrementalRefreshBook(MDIncrementalRefreshBook32 &,
                                    Decoder::Message);
  bool DecodeIncrementalRefreshVolume(MDIncrementalRefreshVolume37 &,
                                      Decoder::Message);
  bool DecodeIncrementalRefreshTrade(MDIncrementalRefreshTrade36 &,
                                     Decoder::Message);
  bool
  DecodeIncrementalRefreshTradeSummary(MDIncrementalRefreshTradeSummary42 &,
                                       Decoder::Message);
  bool DecodeIncrementalRefreshOrderBook(MDIncrementalRefreshOrderBook43 &,
                                         Decoder::Message);
  bool DecodeIncrementalRefreshDailyStatistics(
      MDIncrementalRefreshDailyStatistics33 &, Decoder::Message);
  bool
  DecodeIncrementalRefreshLimitsBanding(MDIncrementalRefreshLimitsBanding34 &,
                                        Decoder::Message);
  bool DecodeIncrementalRefreshSessionStatistics(
      MDIncrementalRefreshSessionStatistics35 &refresh, Decoder::Message);
  bool DecodeSnapshot(SnapshotFullRefresh38 &, Decoder::Message);
  bool DecodeSnapshotFullRefreshOrderbook(SnapshotFullRefreshOrderBook44 &,
                                          Decoder::Message);
  size_t DecodeMessage(char *, size_t);
  size_t DecodeMessageLength(char *, size_t);
  size_t DecodeHeader(MessageHeader &, char *, uint64_t, uint64_t);

  struct MessageContainer {

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

  } messages_;
};
