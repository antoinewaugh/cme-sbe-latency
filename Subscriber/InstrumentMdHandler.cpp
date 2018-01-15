#include "InstrumentMdHandler.h"

#include "Trade.h"
#include <chrono>

static void clear() { std::cout << "\x1B[2J\x1B[H"; }

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

}

void InstrumentMdHandler::ClearState() {
  statechange_ = false;
  book_.Clear();
  implbook_.Clear();
}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshBook32::NoMDEntries &entry, std::uint64_t transacttime) {

  statechange_ = true;

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

  statechange_ = true;

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
  statechange_ = true;
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

  Callback(book_);
}


void InstrumentMdHandler::Callback(DepthBook const& book) {
// handler_.OnQuote(book, 0,0);
}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshVolume37::NoMDEntries &, std::uint64_t transacttime) {

}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshTradeSummary42::NoMDEntries &entry, std::uint64_t transacttime) {

  auto price = entry.mDEntryPx().mantissa() * std::pow(10, entry.mDEntryPx().exponent());
  auto volume = entry.mDEntrySize();
  Trade trade(price, volume);

  handler_.OnTrade(trade, entry.securityID());

}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshDailyStatistics33::NoMDEntries &, std::uint64_t transacttime) {

}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshSessionStatistics35::NoMDEntries &, std::uint64_t transacttime) {

}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshLimitsBanding34::NoMDEntries &, std::uint64_t transacttime) {

}

void InstrumentMdHandler::OnChannelReset() {

}

void InstrumentMdHandler::Commit() {

  if(statechange_) {
    statechange_ = false;
    //  Callback(implbook_);
    Callback(book_);
  }
}



