#include "SymbolFeed.h"

#include <cmath>

SymbolFeed::SymbolFeed(uint64_t securityid, Handler &handler,
                       Decoder &decoder, boost::asio::io_service &io_service,
                   const boost::asio::ip::address &listen_address,
                   const boost::asio::ip::address &multicast_address,
                   const short multicast_port)
    : securityid_(securityid), handler_(handler), decoder_(decoder),
      incrementalA_(decoder, io_service, listen_address, multicast_address, multicast_port),
      incrementalB_(decoder, io_service, listen_address, multicast_address, multicast_port),
      snapshotA_(decoder, io_service, listen_address, multicast_address, multicast_port),
      snapshotB_(decoder, io_service, listen_address, multicast_address, multicast_port) {

  decoder.RegisterCallbacks(
      [this](auto &&val) { this->OnMDIncrementalRefreshBook32(val); },
      [this](auto &&val) {
        this->OnMDIncrementalRefreshDailyStatistics33(val);
      },
      [this](auto &&val) { this->OnMDIncrementalRefreshLimitsBanding34(val); },
      [this](auto &&val) {
        this->OnMDIncrementalRefreshSessionStatistics35(val);
      },
      [this](auto &&val) { this->OnMDIncrementalRefreshTrade36(val); },
      [this](auto &&val) { this->OnMDIncrementalRefreshVolume37(val); },
      [this](auto &&val) { this->OnMDIncrementalRefreshTradeSummary42(val); },
      [this](auto &&val) { this->OnMDIncrementalRefreshOrderBook43(val); },
      [this](auto &&val) { this->OnMDSnapshotFullRefresh38(val); },
      [this](auto &&val) { this->OnMDSnapshotFullRefreshOrderBook44(val); });

  incrementalA_.Join();
  incrementalB_.Join();
  StartRecovery();
}

SymbolFeed::~SymbolFeed() {
  incrementalA_.Leave();
  incrementalB_.Leave();
  snapshotA_.Leave();
  snapshotB_.Leave();
}

void SymbolFeed::StartRecovery() {

  std::cout << "Starting Recovery";

  if (!recoverymode_) {
    recoverymode_ = true;
    book_.Clear();
    seqnum_ = 0; // ensures subsequent incrementals are ignored until snapshot
                 // alignment
    snapshotA_.Join();
    snapshotB_.Join();
  }
}

void SymbolFeed::StopRecovery() {

  std::cout << "Stopping Recovery";

  if (recoverymode_) {
    recoverymode_ = false;
    snapshotA_.Leave();
    snapshotB_.Leave();
  }
}

void SymbolFeed::OnMDSnapshotFullRefresh38(SnapshotFullRefresh38 &refresh) {

  if (!recoverymode_)
    return;

  if (refresh.securityID() != securityid_)
    return;

  auto &entry = refresh.noMDEntries();

  book_.Clear();
  seqnum_ = refresh.lastMsgSeqNumProcessed(); // fast forward seqnum for
                                              // incremental feed alignment

  while (entry.hasNext()) {
    entry.next();

    int level = entry.mDPriceLevel();
    float price = entry.mDEntryPx().mantissa() *
                  std::pow(10, entry.mDEntryPx().exponent());
    int volume = entry.mDEntrySize();

    if (entry.mDEntryType() == MDEntryType::Bid) {
      book_.AddBid(level, price, volume);
    } else if (entry.mDEntryType() == MDEntryType::Offer) {
      book_.AddAsk(level, price, volume);
    }
  }
  handler_.OnQuote(book_);
}

void SymbolFeed::HandleAskEntry(MDUpdateAction::Value action, int level,
                                float price, int volume) {
  switch (action) {
  case MDUpdateAction::New:
    book_.AddAsk(level, price, volume);
    break;
  case MDUpdateAction::Change:
    book_.UpdateAsk(level, price, volume);
    break;
  case MDUpdateAction::Delete:
    book_.DeleteAsk(level, price);
    break;
  case MDUpdateAction::DeleteFrom:
    book_.DeleteAskFrom(level);
    break;
  case MDUpdateAction::DeleteThru:
    book_.DeleteAskThru(level);
    break;
  }
}
void SymbolFeed::HandleBidEntry(MDUpdateAction::Value action, int level,
                                float price, int volume) {
  switch (action) {
  case MDUpdateAction::New:
    book_.AddBid(level, price, volume);
    break;
  case MDUpdateAction::Change:
    book_.UpdateBid(level, price, volume);
    break;
  case MDUpdateAction::Delete:
    book_.DeleteBid(level, price);
    break;
  case MDUpdateAction::DeleteFrom:
    book_.DeleteBidFrom(level);
    break;
  case MDUpdateAction::DeleteThru:
    book_.DeleteBidThru(level);
    break;
  }
}

void SymbolFeed::OnMDIncrementalRefreshBook32(
    MDIncrementalRefreshBook32 &refresh) {

  auto &entry = refresh.noMDEntries();

  while (entry.hasNext()) {

    entry.next();

    if (entry.securityID() != securityid_)
      continue;
    if (entry.rptSeq() != seqnum_ + 1)
      continue;
    if (entry.rptSeq() > seqnum_ + 1) {
      StartRecovery();
      break;
    }

    if (recoverymode_)
      StopRecovery();

    int level = entry.mDPriceLevel();
    float price = entry.mDEntryPx().mantissa() *
                  std::pow(10, entry.mDEntryPx().exponent());
    int volume = entry.mDEntrySize();

    if (entry.mDEntryType() == MDEntryTypeBook::Bid) {
      HandleBidEntry(entry.mDUpdateAction(), level, price, volume);
    } else if (entry.mDEntryType() == MDEntryTypeBook::Offer) {
      HandleAskEntry(entry.mDUpdateAction(), level, price, volume);
    }

    ++seqnum_;
  }
}

void SymbolFeed::OnMDSnapshotFullRefreshOrderBook44(
    SnapshotFullRefreshOrderBook44 &) {}

void SymbolFeed::OnMDIncrementalRefreshOrderBook43(
    MDIncrementalRefreshOrderBook43 &) {}

void SymbolFeed::OnMDIncrementalRefreshDailyStatistics33(
    MDIncrementalRefreshDailyStatistics33 &) {}

void SymbolFeed::OnMDIncrementalRefreshLimitsBanding34(
    MDIncrementalRefreshLimitsBanding34 &) {}

void SymbolFeed::OnMDIncrementalRefreshSessionStatistics35(
    MDIncrementalRefreshSessionStatistics35 &) {}

void SymbolFeed::OnMDIncrementalRefreshTrade36(MDIncrementalRefreshTrade36 &) {}

void SymbolFeed::OnMDIncrementalRefreshVolume37(
    MDIncrementalRefreshVolume37 &) {}

void SymbolFeed::OnMDIncrementalRefreshTradeSummary42(
    MDIncrementalRefreshTradeSummary42 &) {}
