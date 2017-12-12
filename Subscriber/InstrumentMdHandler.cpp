#include "InstrumentMdHandler.h"

#include <chrono>

static void clear() { std::cout << "\x1B[2J\x1B[H"; }

static std::uint64_t NanosecondTimeDelta(uint64_t start, uint64_t stop) {
  return stop - start;
}

static std::uint64_t NanosecondTimeDeltaToNow(uint64_t start) {
  unsigned long ns_timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::system_clock::now().time_since_epoch())
      .count();
  return NanosecondTimeDelta(start, ns_timestamp);
}

void HandleBidEntry(MDIncrementalRefreshBook32::NoMDEntries& entry, DepthBook& book, uint64_t transacttime) {

  auto level = entry.mDPriceLevel();
  auto price = entry.mDEntryPx().mantissa() * std::pow(10, entry.mDEntryPx().exponent());
  auto volume = entry.mDEntrySize();
  auto action = entry.mDUpdateAction();

  switch (action) {
    case MDUpdateAction::New: book.AddBid(level, price, volume); break;
    case MDUpdateAction::Change: book.UpdateBid(level, price, volume); break;
    case MDUpdateAction::Delete: book.DeleteBid(level, price); break;
    case MDUpdateAction::DeleteFrom: book.DeleteBidFrom(level); break;
    case MDUpdateAction::DeleteThru: book.DeleteBidThru(); break;
  }

  clear();

  auto delay_ns = NanosecondTimeDeltaToNow(transacttime);
  auto delay_adj = delay_ns < 0? 0: delay_ns;

  std::cout << "Incremental: " << delay_adj/1000 << " μs" <<  '\n';
  std::cout << book << '\n';
}

void HandleAskEntry(MDIncrementalRefreshBook32::NoMDEntries& entry, DepthBook& book, uint64_t transacttime) {

  auto level = entry.mDPriceLevel();
  auto price = entry.mDEntryPx().mantissa() * std::pow(10, entry.mDEntryPx().exponent());
  auto volume = entry.mDEntrySize();
  auto action = entry.mDUpdateAction();

  switch (action) {
    case MDUpdateAction::New: book.AddAsk(level, price, volume); break;
    case MDUpdateAction::Change: book.UpdateAsk(level, price, volume); break;
    case MDUpdateAction::Delete: book.DeleteAsk(level, price); break;
    case MDUpdateAction::DeleteFrom: book.DeleteAskFrom(level);break;
    case MDUpdateAction::DeleteThru: book.DeleteAskThru();break;
  }

  clear();

  auto delay_ns = NanosecondTimeDeltaToNow(transacttime);
  auto delay_adj = delay_ns < 0? 0: delay_ns;

  std::cout << "Incremental: " << delay_adj/1000 << " μs" <<  '\n';
  std::cout << book << '\n';

}

void InstrumentMdHandler::ClearState() {
  book_.Clear();
  implbook_.Clear();
  // clear trades too? status?
}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshBook32::NoMDEntries &entry, std::uint64_t transacttime) {
  switch(entry.mDEntryType()) {
    case MDEntryTypeBook::Bid:
      HandleBidEntry(entry, book_, transacttime);
      break;
    case MDEntryTypeBook::Offer:
      HandleAskEntry(entry, book_, transacttime);
      break;
    case MDEntryTypeBook::ImpliedBid:
      HandleBidEntry(entry, implbook_, transacttime);
      break;
    case MDEntryTypeBook::ImpliedOffer:
      HandleAskEntry(entry, implbook_, transacttime);
      break;
    case MDEntryTypeBook::BookReset:
      ClearState();
      break;
  }
}


void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshTrade36::NoMDEntries &entry, std::uint64_t transacttime) {
  auto action = entry.mDUpdateAction();
  switch(action) {
    case MDUpdateAction::New:
//      tradehandler_.Add()
      break;
    case MDUpdateAction::Change:
//      tradehandler_.Update()
      break;
    case MDUpdateAction::Delete:
//      tradehandler_.Delete()
      break;
  }
}

//..... Need to expand out all other ::NoMDEntries types

void InstrumentMdHandler::OnSecurityStatus(SecurityStatus30 &status) {
//  statushandler_.....
}


void InstrumentMdHandler::Reset() {
  ClearState();
}

void InstrumentMdHandler::OnSnapshot(SnapshotFullRefresh38 &refresh, std::uint64_t transacttime) {

  ClearState();

  auto &entry = refresh.noMDEntries();
  while (entry.hasNext()) {
    entry.next();

    auto level = entry.mDPriceLevel();
    auto price = entry.mDEntryPx().mantissa() * std::pow(10, entry.mDEntryPx().exponent());
    auto volume = entry.mDEntrySize();

    switch (entry.mDEntryType()) {
      case MDEntryType::Bid:
        book_.AddBid(level, price, volume);
        break;
      case MDEntryType::Offer:
        book_.AddAsk(level, price, volume);
        break;
      case MDEntryType::ImpliedBid:
        implbook_.AddBid(level, price, volume);
        break;
      case MDEntryType::ImpliedOffer:
        implbook_.AddAsk(level, price, volume);
        break;
    }
  }

  clear();

  auto delay_ns = NanosecondTimeDeltaToNow(transacttime);
  auto delay_adj = delay_ns < 0? 0: delay_ns;

  std::cout << "Snapshot: " << delay_adj/1000 << " μs" <<  '\n';
  std::cout << book_ << '\n';
}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshVolume37::NoMDEntries &, std::uint64_t transacttime) {

}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshTradeSummary42::NoMDEntries &, std::uint64_t transacttime) {

}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshDailyStatistics33::NoMDEntries &, std::uint64_t transacttime) {

}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshSessionStatistics35::NoMDEntries &, std::uint64_t transacttime) {

}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshLimitsBanding34::NoMDEntries &, std::uint64_t transacttime) {

}

void InstrumentMdHandler::OnChannelReset() {

}



