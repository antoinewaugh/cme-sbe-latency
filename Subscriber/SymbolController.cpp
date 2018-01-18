#include "SymbolController.h"

namespace sp {
namespace lltp {
namespace cme {

SymbolController::SymbolController(ChannelAccessor* channel): state_(INIT), channel_(channel) {}

void SymbolController::SwitchState(SymbolController::DownloadState oldstate, SymbolController::DownloadState newstate) {
  state_ = newstate;
}

// TODO: identfiy sequence gaps - testing
void SymbolController::OnInstrumentPacket(Packet *packet) {

  if((state_ == INIT && packet->GetSeqNum() == 1) ||
    (state_ == DOWNLOADING))
  {
    while (packet->HasNextMessage()) {
      auto &m = packet->NextMessage();
      auto templateid = m.GetTemplateId();
      if(templateid == MDInstrumentDefinitionFuture27::sbeTemplateId()) {
        OnInstrument<MDInstrumentDefinitionFuture27>(m.Get<MDInstrumentDefinitionFuture27>());
      } else if(templateid == MDInstrumentDefinitionOption41::sbeTemplateId()) {
        OnInstrument<MDInstrumentDefinitionOption41>(m.Get<MDInstrumentDefinitionOption41>());
      } else if(templateid == MDInstrumentDefinitionSpread29::sbeTemplateId()) {
        OnInstrument<MDInstrumentDefinitionSpread29>(m.Get<MDInstrumentDefinitionSpread29>());
      }
    }
  }
}


  std::ostream &operator<<(std::ostream &os, const Instrument &instrument) {
    os << "securityid: " << instrument.securityid << " symbol: " << instrument.symbol << " securitygroupid: "
       << instrument.securitygroupid << " type: " << instrument.type;
    return os;
  }
}}} // namespace
