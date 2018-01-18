#pragma once

#include "ChannelAccessor.h"
#include "Packet.h"
#include "Message.h"
#include <unordered_map>
#include <ostream>
#include <algorithm>

namespace sp {
namespace lltp {
namespace cme {

struct Instrument {
 int securityid;
 std::string symbol;
 std::string securitygroupid;
 std::string type;

  friend std::ostream &operator<<(std::ostream &os, const Instrument &instrument);
};

class SymbolController {
public:
  SymbolController() = default;
  SymbolController(ChannelAccessor* channel);
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
  ChannelAccessor* channel_;
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
  auto securitygroupid = std::string(definition.getSecurityGroupAsString().c_str());
  auto type = std::string(definition.getSecurityTypeAsString().c_str());

  if(definition.securityUpdateAction() == SecurityUpdateAction::Add) {
    auto instrument = Instrument{securityid, symbol, securitygroupid, type};
    instruments_.emplace(symbol, instrument);
    std::cout << instrument << '\n';
  } else if(definition.securityUpdateAction() == SecurityUpdateAction::Modify) {
    auto result = instruments_.find(symbol);
    if (result != std::end(instruments_)) {
      auto instrument = result->second;
      instrument.securityid = securityid;
      instrument.securitygroupid = securitygroupid;
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

// namespace

  // todo:
  // add / delete / modify instrument map/vector
  // add support for HandleInsturmentMessage to be called from incremental feed
  // unsubscribe from instrument feed once contracts downloaded
  // consider larger scale missing packets on instrument download etc, message seq number tracking...
  // add static routes
  // test..



