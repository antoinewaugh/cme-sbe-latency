#include "InstrumentMdHandler.h"

void HandleBidEntry(MDIncrementalRefreshBook32::NoMDEntries& entry, DepthBook& book) {

  auto level = entry.mDPriceLevel();
  auto price = entry.mDEntryPx().mantissa() * std::pow(10, entry.mDEntryPx().exponent());
  auto volume = entry.mDEntrySize();
  auto action = entry.mDUpdateAction();

  switch (action) {
    case MDUpdateAction::New: book.AddBid(level, price, volume); break;
    case MDUpdateAction::Change: book.UpdateBid(level, price, volume);break;
    case MDUpdateAction::Delete: book.DeleteBid(level, price); break;
    case MDUpdateAction::DeleteFrom: book.DeleteBidFrom(level);break;
    case MDUpdateAction::DeleteThru: book.DeleteBidThru();break;
  }
}

void HandleAskEntry(MDIncrementalRefreshBook32::NoMDEntries& entry, DepthBook& book) {

  auto level = entry.mDPriceLevel();
  auto price = entry.mDEntryPx().mantissa() * std::pow(10, entry.mDEntryPx().exponent());
  auto volume = entry.mDEntrySize();
  auto action = entry.mDUpdateAction();

  switch (action) {
    case MDUpdateAction::New: book.AddAsk(level, price, volume); break;
    case MDUpdateAction::Change: book.UpdateAsk(level, price, volume);break;
    case MDUpdateAction::Delete: book.DeleteAsk(level, price); break;
    case MDUpdateAction::DeleteFrom: book.DeleteAskFrom(level);break;
    case MDUpdateAction::DeleteThru: book.DeleteAskThru();break;
  }
}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshBook32::NoMDEntries &entry) {
  switch(entry.mDEntryType()) {
    case MDEntryTypeBook::Bid:
      HandleBidEntry(entry, book_);
      break;
    case MDEntryTypeBook::Offer:
      HandleAskEntry(entry, book_);
      break;
    case MDEntryTypeBook::ImpliedBid:
      HandleBidEntry(entry, implbook_);
      break;
    case MDEntryTypeBook::ImpliedOffer:
      HandleAskEntry(entry, implbook_);
      break;
    case MDEntryTypeBook::BookReset:
      book_.Clear();
      implbook_.Clear();
      break;
  }
}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshTrade36::NoMDEntries &entry) {
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

void InstrumentMdHandler::OnSecurityStatus(SecurityStatus30 &status) {
//  statushandler_.....
}


