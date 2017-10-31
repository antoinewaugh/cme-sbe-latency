#include "SymbolFeed.h"

SymbolFeed::SymbolFeed(uint64_t securityid, Handler &handler, Decoder &decoder)
    : securityid_(securityid), handler_(handler), decoder_(decoder) {
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
  if (!recoverymode_) {
    recoverymode_ = true;
    book_.clear();
    snapshotA_.join();
    snapshotB_.join();
  }
}

void SymbolFeed::StopRecovery() {
  if (recoverymode_) {
    recoverymode_ = false;
    snapshotA_.leave();
    snapshotB_.leave();
  }
}

void SymbolFeed::OnMDSnapshotFullRefresh38(SnapshotFullRefresh38 &refresh) {

  auto &entry = refresh.noMDEntries();

  if (refresh.securityID() == securityid_) {
    book_.clear(); // Confirm blatent clearing of book okay in this instance,
                   // race conditions??
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
  }

  handler_.OnQuote(book_);
}

void SymbolFeed::OnMDIncrementalRefreshBook32(MDIncrementalRefreshBook32 &) {}
