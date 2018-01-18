#include "InstrumentMdHandler.h"
#include "Trade.h"
#include <chrono>

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

// Only used by partner cme exchanges
void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshTrade36::NoMDEntries &entry, std::uint64_t transacttime) {}

void InstrumentMdHandler::OnSecurityStatus(SecurityStatus30 &status) {

  auto TranslateStatus = [=](SecurityStatus30 s) -> sp::lltp::cme::SecurityStatus {
    switch(s.securityTradingStatus()) {
      case SecurityTradingStatus::TradingHalt: return SecurityStatus(SecurityStatus::TradingHalt);
      case SecurityTradingStatus::Close: return SecurityStatus(SecurityStatus::Close);
      case SecurityTradingStatus::NewPriceIndication: return SecurityStatus(SecurityStatus::NewPriceIndication);
      case SecurityTradingStatus::ReadyToTrade: return SecurityStatus(SecurityStatus::ReadyToTrade);
      case SecurityTradingStatus::NotAvailableForTrading: return SecurityStatus(SecurityStatus::NotAvailableForTrading);
      case SecurityTradingStatus::UnknownorInvalid: return SecurityStatus(SecurityStatus::UnknownorInvalid);
      case SecurityTradingStatus::PreOpen: return SecurityStatus(SecurityStatus::PreOpen);
      case SecurityTradingStatus::PreCross: return SecurityStatus(SecurityStatus::PreCross);
      case SecurityTradingStatus::Cross: return SecurityStatus(SecurityStatus::Cross);
      case SecurityTradingStatus::PostClose: return SecurityStatus(SecurityStatus::PostClose);
      case SecurityTradingStatus::NoChange: return SecurityStatus(SecurityStatus::NoChange);
      case SecurityTradingStatus::NULL_VALUE: return SecurityStatus(SecurityStatus::NULL_VALUE);
    }
  };

  handler_.OnStatus(0, TranslateStatus(status));
}

void InstrumentMdHandler::Reset() {
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

  handler_.OnQuote(0, book_, 0);
}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshTradeSummary42::NoMDEntries &entry, std::uint64_t transacttime) {

  auto price = entry.mDEntryPx().mantissa() * std::pow(10, entry.mDEntryPx().exponent());
  auto volume = entry.mDEntrySize();
  Trade trade(price, volume);

  handler_.OnTrade(entry.securityID(), trade);

}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshVolume37::NoMDEntries &entry, std::uint64_t transacttime) {
  statistics_.clearedvolume = entry.mDEntrySize();
  handler_.OnStatistics(entry.securityID(), statistics_);
}

// could be problematic if entry doesnt have px / size?
void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshDailyStatistics33::NoMDEntries &entry, std::uint64_t transacttime) {
  auto price = entry.mDEntryPx().mantissa() * std::pow(10, entry.mDEntryPx().exponent());
  auto volume = entry.mDEntrySize();
  switch(entry.mDEntryType()) {
    case MDEntryTypeDailyStatistics::SettlementPrice: statistics_.settlement= price; break;
    case MDEntryTypeDailyStatistics::FixingPrice: statistics_.fixingprice = price; break;
    case MDEntryTypeDailyStatistics::ClearedVolume: statistics_.clearedvolume = volume; break;
    case MDEntryTypeDailyStatistics::OpenInterest: statistics_.openinterest = volume; break;
  }
  handler_.OnStatistics(entry.securityID(), statistics_);
}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshSessionStatistics35::NoMDEntries &entry, std::uint64_t transacttime) {

  auto price = entry.mDEntryPx().mantissa() * std::pow(10, entry.mDEntryPx().exponent());

  switch(entry.mDEntryType()) {
    case MDEntryTypeStatistics::OpenPrice: statistics_.openingprice = price; break;
    case MDEntryTypeStatistics::LowTrade: statistics_.sessionlowprice= price; break;
    case MDEntryTypeStatistics::HighTrade: statistics_.sessionhighprice= price; break;
    case MDEntryTypeStatistics::LowestOffer: statistics_.sessionlowask= price; break;
    case MDEntryTypeStatistics::HighestBid: statistics_.sessionhighbid= price; break;
  }
  handler_.OnStatistics(entry.securityID(), statistics_);
}

void InstrumentMdHandler::OnIncremental(MDIncrementalRefreshLimitsBanding34::NoMDEntries &entry, std::uint64_t transacttime) {

  auto highprice = entry.highLimitPrice().mantissa() * std::pow(10, entry.highLimitPrice().exponent());
  auto lowprice = entry.lowLimitPrice().mantissa() * std::pow(10, entry.lowLimitPrice().exponent());

  statistics_.sessionlowprice = lowprice;
  statistics_.sessionhighprice= highprice;

  handler_.OnStatistics(entry.securityID(), statistics_);
}

void InstrumentMdHandler::OnChannelReset() {
  ClearState();

  handler_.OnQuote(0, book_, 0);
  handler_.OnQuote(0, implbook_, 0);
}

void InstrumentMdHandler::Commit() {

  if(statechange_) {
    statechange_ = false;
    handler_.OnQuote(0, implbook_, 0);
    handler_.OnQuote(0, book_, 0);
  }
}



