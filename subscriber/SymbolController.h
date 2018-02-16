#pragma once

#include "ChannelContext.h"
#include "Packet.h"
#include "Message.h"
#include <unordered_map>
#include <ostream>
#include <algorithm>
#include "Instrument.h"


namespace sp {
namespace lltp {
namespace cme {

using sp::lltp::cme::Instrument;

class SymbolController {
public:
  SymbolController() = default;
  SymbolController(ChannelContext* channel);
  Instrument GetInstrument(std::string const& symbol);
  void OnInstrumentPacket(Packet*);

private:

  enum DownloadState {
    INIT,
    DOWNLOADING,
    DOWNLOADED,
  };

  void HandlePacket(Packet*);
  void ResetState();
  void SwitchState(DownloadState oldstate, DownloadState newstate);
  bool DownloadComplete() { return processedreports_ >= totalnumreports_;}
  template<typename T> void HandleDefinition(T& definition);
  template<typename T> void OnInstrument(T&);
  DownloadState state_;
  ChannelContext* channel_;
  int totalnumreports_ = 0;
  int processedreports_ = 0;
  int expectedseqnum_ = 0;
  std::unordered_map<std::string, Instrument> instruments_;
};


template<typename T>
void SymbolController::HandleDefinition(T& definition) {

  auto securitystatus = definition.mDSecurityTradingStatus();
  auto securityid = definition.securityID();
  auto symbol = std::string(definition.getSymbolAsString().c_str());                  // std::string wrapper required as AsString returns string containing fixed length
  auto securitygroup = std::string(definition.getSecurityGroupAsString().c_str());
  auto type = std::string(definition.getSecurityTypeAsString().c_str());
  auto marketsegmentid = definition.marketSegmentID();

  uint64_t  activationdatetime;
  uint64_t  expirydatetime;

  auto event = definition.noEvents();
  while(event.hasNext())  {
    event.next();
    uint64_t time = event.eventTime();
    switch(event.eventType()) {
      case EventType::Activation:  activationdatetime = event.eventTime();
      case EventType::LastEligibleTradeDate: expirydatetime = event.eventTime();
    }
  }

  int marketdepth = 0;
  int implmarketdepth = 0;

  auto feed = definition.noMDFeedTypes();
  while(feed.hasNext()) {
    feed.next();
    auto type = feed.getMDFeedTypeAsString();
    if(type == "GBX") {
      marketdepth = feed.marketDepth();
    } else if (type == "GBI") {
      implmarketdepth = feed.marketDepth();
    }
  }

  if(definition.securityUpdateAction() == SecurityUpdateAction::Add) {
    auto instrument = Instrument{securityid, symbol, securitygroup, type, activationdatetime, expirydatetime, marketsegmentid, marketdepth, implmarketdepth};
    instruments_.emplace(symbol, instrument);
    std::cout << instrument << '\n';
  } else if(definition.securityUpdateAction() == SecurityUpdateAction::Modify) {
    auto result = instruments_.find(symbol);
    if (result != std::end(instruments_)) {
      auto instrument = result->second;
      instrument.securityid = securityid;
      instrument.securitygroup = securitygroup;
      instrument.type = type;
    }
  } else if(definition.securityUpdateAction() == SecurityUpdateAction::Delete) {
    auto result = instruments_.find(symbol);
    if (result != std::end(instruments_)) {
      instruments_.erase(result);
    }
  }
  processedreports_++;
}

template<typename T>
void SymbolController::OnInstrument(T& definition) {
  if(state_ == INIT) {
    totalnumreports_ = definition.totNumReports();
    SwitchState(INIT, DOWNLOADING);
    HandleDefinition(definition);
  } else if(state_ == DOWNLOADING) {
    HandleDefinition(definition);
    if(DownloadComplete()) {
      SwitchState(DOWNLOADING, DOWNLOADED);
    }
  }
}

}}}



