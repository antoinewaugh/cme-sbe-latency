#pragma once

#include <boost/asio.hpp>
#include "boost/bind.hpp"

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
  size_t decode_incremental_refresh_book(MDIncrementalRefreshBook32 &, char *,
                                         uint64_t, std::uint64_t, std::uint64_t,
                                         std::uint64_t);
  size_t decode_incremental_refresh_volume();
  size_t decode_incremental_refresh_trade();
  size_t decode_incremental_refresh_order_book();
  size_t decode_snapshot(SnapshotFullRefresh38&, char *,
                                         uint64_t, std::uint64_t, std::uint64_t,
                                         std::uint64_t);
  size_t decode_message(uint64_t);
  size_t decode_packet(size_t);
  size_t decode_header(MessageHeader &, char *, uint64_t, uint64_t);

  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint sender_endpoint_;

  enum { max_length = 4096 };
  char data_[max_length];

  MessageHeader header_;
  SnapshotFullRefresh38 snapshot_full_;
  SnapshotFullRefreshOrderBook44 snapshot_full_order_book_;
  MDIncrementalRefreshBook32 incremental_refresh_book_;
  MDIncrementalRefreshVolume37 incremental_volume_;
  MDIncrementalRefreshTradeSummary42 incremental_trade_;
  MDIncrementalRefreshOrderBook43 incremental_order_book_;

  OrderBook book_;

 public:
  void handle_packet(const boost::system::error_code &, size_t);
};
