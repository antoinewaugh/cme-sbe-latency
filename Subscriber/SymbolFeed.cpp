#include "SymbolFeed.h"

#include <cmath>

// SymbolFeed::SymbolFeed(uint64_t securityid, Handler &handler, Decoder
// &decoder)
SymbolFeed::SymbolFeed(uint64_t securityid, Handler &handler)
    : securityid_(securityid), handler_(handler) {
  incrementalA_.join();
  incrementalB_.join();
  StartRecovery();
}

SymbolFeed::~SymbolFeed() {
  incrementalA_.leave();
  incrementalB_.leave();
  snapshotA_.leave();
  snapshotB_.leave();
}

void SymbolFeed::StartRecovery() {

  std::cout << "Starting Recovery";

  if (!recoverymode_) {
    recoverymode_ = true;
    book_.clear();
    seqnum_ = 0; // ensures subsequent incrementals are ignored until snapshot
                 // alignment
    snapshotA_.join();
    snapshotB_.join();
  }
}

void SymbolFeed::StopRecovery() {

  std::cout << "Stopping Recovery";

  if (recoverymode_) {
    recoverymode_ = false;
    snapshotA_.leave();
    snapshotB_.leave();
  }
}

void SymbolFeed::OnMDSnapshotFullRefresh38(SnapshotFullRefresh38 &refresh) {

  if (!recoverymode_)
    return;

  if (refresh.securityID() != securityid_)
    return;

  auto &entry = refresh.noMDEntries();

  book_.clear();
  seqnum_ = refresh.lastMsgSeqNumProcessed(); // fast forward seqnum for
                                              // incremental feed alignment

  while (entry.hasNext()) {
    entry.next();

    int level = entry.mDPriceLevel();
    float price = entry.mDEntryPx().mantissa() *
                  std::pow(10, entry.mDEntryPx().exponent());
    int volume = entry.mDEntrySize();

    if (entry.mDEntryType() == MDEntryType::Bid) {
      book_.add_bid(level, price, volume);
    } else if (entry.mDEntryType() == MDEntryType::Offer) {
      book_.add_ask(level, price, volume);
    }
  }
  handler_.OnQuote(book_);
}

void SymbolFeed::HandleAskEntry(MDUpdateAction::Value action, int level,
                                float price, int volume) {
  switch (action) {
  case MDUpdateAction::New:
    book_.add_ask(level, price, volume);
    break;
  case MDUpdateAction::Change:
    book_.update_ask(level, price, volume);
    break;
  case MDUpdateAction::Delete:
    book_.delete_ask(level, price);
    break;
  case MDUpdateAction::DeleteFrom:
    book_.delete_ask_from(level);
    break;
  case MDUpdateAction::DeleteThru:
    book_.delete_ask_thru(level);
    break;
  }
}
void SymbolFeed::HandleBidEntry(MDUpdateAction::Value action, int level,
                                float price, int volume) {
  switch (action) {
  case MDUpdateAction::New:
    book_.add_bid(level, price, volume);
    break;
  case MDUpdateAction::Change:
    book_.update_bid(level, price, volume);
    break;
  case MDUpdateAction::Delete:
    book_.delete_bid(level, price);
    break;
  case MDUpdateAction::DeleteFrom:
    book_.delete_bid_from(level);
    break;
  case MDUpdateAction::DeleteThru:
    book_.delete_bid_thru(level);
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
