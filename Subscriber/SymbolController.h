#pragma once

#include "ChannelAccessor.h"
#include "Packet.h"
#include "Message.h"

namespace sp {
namespace lltp {
namespace cme {

class SymbolController {
public:
  SymbolController();
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
};


template<typename T>
void SymbolController::HandleDefinition(T& definition) {
  switch(definition.securityUpdateAction()) {
    case SecurityUpdateAction::Add: break;
    case SecurityUpdateAction::Delete: break;
    case SecurityUpdateAction::Modify: break;
  }
  processedreports_++;
}

template<typename T>
void SymbolController::OnInstrument(T& definition) {
  if(state_ == INIT) {
    totalnumreports_ = definition.totalNumReports();
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


