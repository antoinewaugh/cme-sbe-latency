#include "ChannelInstrumentManager.h"

#include "Message.h"

#include "sbe/MDInstrumentDefinitionFuture27.h"
#include "sbe/MDInstrumentDefinitionOption41.h"
#include "sbe/MDInstrumentDefinitionSpread29.h"

template<typedef T>
void ChannelInstrumentManager::OnMessage(T* secdef) {

  //callbacks.NotifySecurityDefinitionListeners();

  auto securityid = secdef.securityID();
  auto search = instruments_.find(securityid);

  if(search == instruments_.end()) {
    AddInstrument(securityid, secdef);
  } else {
    UpdateInstrument(securityid, search->second);
  }

  if(CanStopInstrumentFeed()) {
    context_->StopInstrumentFeed();
  }
}

bool ChannelInstrumentManager::CanStopInstrumentFeed() {
  return false;
}

void OnPacket(Packet *packet) {


}
