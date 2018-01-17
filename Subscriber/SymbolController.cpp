#include "SymbolController.h"

namespace sp {
namespace lltp {
namespace cme {

SymbolController::SymbolController(): state_(INIT) {}

void SymbolController::SwitchState(SymbolController::DownloadState oldstate, SymbolController::DownloadState newstate) {
  state_ = newstate;
}

void SymbolController::OnInstrumentPacket(Packet *packet) {
  while(packet->HasNextMessage()) {
    auto& m = packet->NextMessage();
//    auto& message = m.Get<T>();
//    OnInstrument(message);
  }
}


}}} // namespace
