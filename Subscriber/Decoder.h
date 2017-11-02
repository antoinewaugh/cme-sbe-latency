#pragma once

#include "OrderBook.h"

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

#include <functional>

constexpr int kMsgSize = 2;
constexpr int kByteOffest = 12;
constexpr int kMsgHeaderVersion = 0;

using namespace sbe;

class Decoder {
public:
  struct Message {
    char *buffer;
    uint16_t offset;
    uint16_t block_length;
    uint16_t version;
    uint16_t buffer_length;
  };

  size_t DecodePacket(char *, size_t);
  int RegisterCallbacks(
      std::function<void(MDIncrementalRefreshBook32 &)>,
      std::function<void(MDIncrementalRefreshDailyStatistics33 &)>,
      std::function<void(MDIncrementalRefreshLimitsBanding34 &)>,
      std::function<void(MDIncrementalRefreshSessionStatistics35 &)>,
      std::function<void(MDIncrementalRefreshTrade36 &)>,
      std::function<void(MDIncrementalRefreshVolume37 &)>,
      std::function<void(MDIncrementalRefreshTradeSummary42 &)>,
      std::function<void(MDIncrementalRefreshOrderBook43 &)>,
      std::function<void(SnapshotFullRefresh38 &)>,
      std::function<void(SnapshotFullRefreshOrderBook44 &)>);

private:
  size_t decode_incremental_refresh_book(MDIncrementalRefreshBook32 &,
                                         Decoder::Message);
  size_t decode_incremental_refresh_volume(MDIncrementalRefreshVolume37 &,
                                           Decoder::Message);
  size_t decode_incremental_refresh_trade(MDIncrementalRefreshTradeSummary42 &,
                                          Decoder::Message);
  size_t
  decode_incremental_refresh_order_book(MDIncrementalRefreshOrderBook43 &,
                                        Decoder::Message);
  size_t decode_snapshot(SnapshotFullRefresh38 &, Decoder::Message);
  size_t decode_message(char *, size_t);
  size_t decode_message_length(char *, size_t);
  size_t decode_header(MessageHeader &, char *, uint64_t, uint64_t);

  MessageHeader header_;
  SnapshotFullRefresh38 snapshot_full_;
  SnapshotFullRefreshOrderBook44 snapshot_full_order_book_;
  MDIncrementalRefreshBook32 incremental_refresh_book_;
  MDIncrementalRefreshVolume37 incremental_volume_;
  MDIncrementalRefreshTradeSummary42 incremental_trade_;
  MDIncrementalRefreshOrderBook43 incremental_order_book_;

  std::function<void(MDIncrementalRefreshBook32 &)> cb_book_;
  std::function<void(MDIncrementalRefreshDailyStatistics33 &)>
      cb_dailystatistics_;
  std::function<void(MDIncrementalRefreshLimitsBanding34 &)> cb_limitsbanding_;
  std::function<void(MDIncrementalRefreshSessionStatistics35 &)>
      cb_sessionstatistics_;
  std::function<void(MDIncrementalRefreshTrade36 &)> cb_trade_;
  std::function<void(MDIncrementalRefreshVolume37 &)> cb_volume_;
  std::function<void(MDIncrementalRefreshTradeSummary42 &)> cb_tradesummary_;
  std::function<void(MDIncrementalRefreshOrderBook43 &)> cb_orderbook_;
  std::function<void(SnapshotFullRefresh38 &)> cb_snapshotfull_;
  std::function<void(SnapshotFullRefreshOrderBook44 &)> cb_snapshotorderbook_;

  bool registered_ = false;
};
