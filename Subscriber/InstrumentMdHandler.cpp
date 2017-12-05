#include "InstrumentMdHandler.h"


void InstrumentMdHandler::OnIncrementalRefreshBookEntry(MDIncrementalRefreshBook32::NoMDEntries &entry) {
  auto type = entry.mDEntryType();
  switch(type) {
    case MDEntryTypeBook::Bid:
      //
      break;
    case MDEntryTypeBook::Offer:
      //
      break;
    case MDEntryTypeBook::ImpliedBid:
      //
      break;
    case MDEntryTypeBook::ImpliedOffer:
      //
      break;

    case MDEntryTypeBook::BookReset:
      //
      break;
  }
}

void InstrumentMdHandler::OnIncrementalRefreshTradeEntry(MDIncrementalRefreshTrade36::NoMDEntries &entry) {
  auto action = entry.mDUpdateAction();
  switch(action) {
    case MDUpdateAction::New:
      tradehandler_.Add()
      break;
    case MDUpdateAction::Change:
      tradehandler_.Update()
      break;
    case MDUpdateAction::Delete:
      tradehandler_.Delete()
      break
  }
}

