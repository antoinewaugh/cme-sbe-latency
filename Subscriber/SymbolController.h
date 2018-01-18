#pragma once

#include "ChannelAccessor.h"
#include "Packet.h"
#include "Message.h"
#include <unordered_map>
#include <ostream>

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
  void OnInstrumentPacket(Packet*);

private:

  enum DownloadState {
    INIT,
    DOWNLOADING,
    DOWNLOADED,
  };

  void SwitchState(DownloadState oldstate, DownloadState newstate);
  bool DownloadComplete() { return processedreports_ >= totalnumreports_;}
  template<typename T> void HandleDefinition(T& definition);
  template<typename T> void OnInstrument(T&);
  DownloadState state_;
  ChannelAccessor* channel_;
  int totalnumreports_ = 0;
  int processedreports_ = 0;
  std::unordered_map<std::string, Instrument> instruments_;
};


template<typename T>
void SymbolController::HandleDefinition(T& definition) {

    auto securitystatus = definition.mDSecurityTradingStatus();
    auto securityid = definition.securityID();
    auto symbol = definition.getSymbolAsString();
    auto securitygroupid = definition.getSecurityGroupAsString();
    auto type = definition.getSecurityTypeAsString();

  switch(definition.securityUpdateAction()) {
    case SecurityUpdateAction::Add:
      instruments_.emplace(symbol, Instrument{securityid, symbol, securitygroupid, type});
      for (auto [_, v]: instruments_){
       std::cout << v << std::endl;
   }
       break;
//    case SecurityUpdateAction::Delete: instruments_.erase(symbol); break;
//    case SecurityUpdateAction::Delete: std::cout<<"Delete instrument def" << '\n';break;
//    case SecurityUpdateAction::Modify: std::cout<<"Modify instrument def" << '\n';break;
  }
  processedreports_++;
}

template<typename T>
void SymbolController::OnInstrument(T& definition) {
  if(state_ == INIT) {
    totalnumreports_ = definition.totNumReports();
    SwitchState(INIT, DOWNLOADING);
    HandleDefinition(definition);
  }
  if(state_ == DOWNLOADING) {
    HandleDefinition(definition);
    if(DownloadComplete()) {
      SwitchState(DOWNLOADING, DOWNLOADED);
      channel_->UnsubscribeInstrumentDownload();
    }
  }
}

}}} // namespace
//template<typename T>
//  void HandleInstrumentMessage(Message& m) {
//    auto& message = m.Get<T>();
//
//    switch(message.securityUpdateAction()) {
//      case SecurityUpdateAction::Add: break;
//      case SecurityUpdateAction::Delete: break;
//      case SecurityUpdateAction::Modify: break;
//    }

    // todo:
    // add / delete / modify instrument map/vector
    // add support for HandleInsturmentMessage to be called from incremental feed
    // unsubscribe from instrument feed once contracts downloaded
    // consider larger scale missing packets on instrument download etc, message seq number tracking...
    // add static routes
    // test..



//  }

