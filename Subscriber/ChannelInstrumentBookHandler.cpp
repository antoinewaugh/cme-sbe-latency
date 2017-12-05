//
// Created by antoine on 5/12/17.
//

#include "ChannelInstrumentBookHandler.h"

void ChannelInstrumentBookHandler::ApplyIncrementalBidEntry(MDIncrementalRefreshBook32::NoMDEntries &entry) {

  auto action = entry.mDUpdateAction();
  auto level = entry.mDPriceLevel();
  auto price = entry.mDEntryPx().mantissa() *
                std::pow(10, entry.mDEntryPx().exponent());
  auto volume = entry.mDEntrySize();

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
      book_.DeleteBidThru();
      break;
  }
}

void ChannelInstrumentBookHandler::ApplyIncrementalAskEntry(MDIncrementalRefreshBook32::NoMDEntries &entry) {

  auto action = entry.mDUpdateAction();
  auto level = entry.mDPriceLevel();
  auto price = entry.mDEntryPx().mantissa() *
               std::pow(10, entry.mDEntryPx().exponent());
  auto volume = entry.mDEntrySize();

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
      book_.DeleteAskThru();
      break;
  }
}

void ChannelInstrumentBookHandler::ApplySnapshotBidEntry(SnapshotFullRefresh38::NoMDEntries &entry) {

  auto level = entry.mDPriceLevel();
  auto price = entry.mDEntryPx().mantissa() *
               std::pow(10, entry.mDEntryPx().exponent());
  auto volume = entry.mDEntrySize();

  book_.AddBid(level, price, volume);
}

void ChannelInstrumentBookHandler::ApplySnapshotAskEntry(SnapshotFullRefresh38::NoMDEntries &entry) {

  auto level = entry.mDPriceLevel();
  auto price = entry.mDEntryPx().mantissa() *
               std::pow(10, entry.mDEntryPx().exponent());
  auto volume = entry.mDEntrySize();

  book_.AddAsk(level, price, volume);
}

void ChannelInstrumentBookHandler::Clear() {
  book_.Clear();
}

DepthBook& ChannelInstrumentBookHandler::GetBook() {
  return book_;
}
