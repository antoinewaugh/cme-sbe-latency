#pragma once

#include "DepthBook.h"
#include "Handler.h"

#include <functional>

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

using namespace sbe;

constexpr int kMsgSize = 2;
constexpr int kByteOffest = 12;
constexpr int kMsgHeaderVersion = 0;

enum SeqNumStatus { Synchronized, Unsynchronised };

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
          std::function<void(SeqNumStatus)> OnSeqNumStatus);

private:

  bool recoverymode_ = false;
  int seqnum_ = 0;
  uint64_t securityid_;

  Handler handler_;
  DepthBook book_;

  std::function<void(SeqNumStatus)> cb_seqnumstatus_;

  void StartRecovery();
  void StopRecovery();

  template <class T> bool CheckStream(T);

  void HandleBidEntry(MDUpdateAction::Value action, int level, float price,
                      int volume);
  void HandleAskEntry(MDUpdateAction::Value action, int level, float price,
                      int volume);

  bool DecodeIncrementalRefreshBook(MDIncrementalRefreshBook32 &,
                                    Decoder::Message);
  bool DecodeIncrementalRefreshVolume(MDIncrementalRefreshVolume37 &,
                                      Decoder::Message);
  bool DecodeIncrementalRefreshTrade(MDIncrementalRefreshTradeSummary42 &,
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

  size_t DecodeMessage(char *, size_t);
  size_t DecodeMessageLength(char *, size_t);
  size_t DecodeHeader(MessageHeader &, char *, uint64_t, uint64_t);

  MessageHeader header_;
  SnapshotFullRefresh38 snapshot_full_;
  SnapshotFullRefreshOrderBook44 snapshot_full_order_book_;
  MDIncrementalRefreshBook32 incremental_refresh_book_;
  MDIncrementalRefreshVolume37 incremental_volume_;
  MDIncrementalRefreshTradeSummary42 incremental_trade_;
  MDIncrementalRefreshOrderBook43 incremental_order_book_;
};
