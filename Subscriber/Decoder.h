#pragma once

#include "sbe/MDIncrementalRefreshBook32.h"
#include "sbe/MDIncrementalRefreshOrderBook43.h"
#include "sbe/MDIncrementalRefreshTradeSummary42.h"
#include "sbe/MDIncrementalRefreshVolume37.h"
#include "sbe/MessageHeader.h"
#include "sbe/SnapshotFullRefresh38.h"
#include "sbe/SnapshotFullRefreshOrderBook44.h"

#include "OrderBook.h"

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
  size_t decode_packet(char *, size_t);

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

  void handle_bid_entry(MDUpdateAction::Value, int, float, int);
  void handle_ask_entry(MDUpdateAction::Value, int, float, int);

  MessageHeader header_;
  SnapshotFullRefresh38 snapshot_full_;
  SnapshotFullRefreshOrderBook44 snapshot_full_order_book_;
  MDIncrementalRefreshBook32 incremental_refresh_book_;
  MDIncrementalRefreshVolume37 incremental_volume_;
  MDIncrementalRefreshTradeSummary42 incremental_trade_;
  MDIncrementalRefreshOrderBook43 incremental_order_book_;

  // Book being populated by decoder
  OrderBook book_;

};
