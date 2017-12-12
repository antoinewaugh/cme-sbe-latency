#include "ChannelController.h"

#include <iostream>
void ChannelController::HandleSnapshotMessage(Message& m) {
  auto& snapshot = m.Get<SnapshotFullRefresh38>();
  auto inst_controller = GetInstrumentController(snapshot.securityID());
  if(inst_controller) {
    inst_controller->OnSnapshot(snapshot);
  }
}

InstrumentController* ChannelController::GetInstrumentController(uint32_t securityid) {
  std::cout << "getInstrumentController :" << securityid << '\n';
  auto search = instrument_controllers_.find(securityid);
  if(search != std::end(instrument_controllers_)){
    return &(search->second);
  }
  std::cout << "instrument_controllers_::" << instrument_controllers_.size() << '\n';
  return nullptr;
}

void ChannelController::HandleIncrementalSecurityStatus(Message& m) {
  auto& message = m.Get<SecurityStatus30>();
  // if group available, propagate status event to all instruments
  // (receiving instrument controllers check for group relevance)
  if(*message.securityGroup() != message.securityGroupNullValue()) { // need to confirm this
    for(auto inst_controller : instrument_controllers_) {
      inst_controller.second.OnSecurityStatus(message);
    }
  } else {
    auto inst_controller = GetInstrumentController(message.securityID());
    if(inst_controller) {
      inst_controller->OnSecurityStatus(message);
    }
  }
}

void ChannelController::OnIncrementalMessage(Message& m) {
  auto templateid = m.GetTemplateId();
  if(templateid == MDIncrementalRefreshBook32::sbeTemplateId()) {
    HandleIncrementalMessage<MDIncrementalRefreshBook32>(m);
  } else if(templateid == MDIncrementalRefreshDailyStatistics33::sbeTemplateId()) {
    HandleIncrementalMessage<MDIncrementalRefreshDailyStatistics33>(m);
  } else if(templateid == MDIncrementalRefreshLimitsBanding34::sbeTemplateId()) {
    HandleIncrementalMessage<MDIncrementalRefreshLimitsBanding34>(m);
  } else if(templateid == MDIncrementalRefreshSessionStatistics35::sbeTemplateId()) {
    HandleIncrementalMessage<MDIncrementalRefreshSessionStatistics35>(m);
  } else if(templateid == MDIncrementalRefreshTrade36::sbeTemplateId()) {
    HandleIncrementalMessage<MDIncrementalRefreshTrade36>(m);
  } else if(templateid == MDIncrementalRefreshTradeSummary42::sbeTemplateId()) {
    HandleIncrementalMessage<MDIncrementalRefreshTradeSummary42>(m);
  } else if(templateid == MDIncrementalRefreshVolume37::sbeTemplateId()) {
    HandleIncrementalMessage<MDIncrementalRefreshVolume37>(m);
  } else if(templateid == QuoteRequest39::sbeTemplateId()) {
    HandleIncrementalQuoteRequest(m);
  } else if(templateid == SecurityStatus30::sbeTemplateId()) {
    HandleIncrementalSecurityStatus(m);
  }
}

void ChannelController::OnIncrementalPacket(Packet *packet) {
  while(packet->HasNextMessage()) {
    auto& message = packet->NextMessage();
    OnIncrementalMessage(message);
  }
}

void ChannelController::OnSnapshotPacket(Packet *packet) {
  while(packet->HasNextMessage()) {
    auto& message = packet->NextMessage();
    HandleSnapshotMessage(message);
  }
}

void ChannelController::Subscribe(uint32_t securityid) {
  auto inst_controller = GetInstrumentController(securityid);
  if(inst_controller == nullptr) {
    std::cout << "Subscribing : " << securityid << '\n';
    instrument_controllers_.emplace(securityid, InstrumentController(securityid, std::string(), channel_));
  }
}

void ChannelController::Unsubscribe(uint32_t securityid) {

}

ChannelController::ChannelController(ChannelAccessor *channel):channel_(channel) {

}

void ChannelController::AddOutOfSyncInstrument(uint32_t securityid) {
  //
}

bool ChannelController::RemoveOutOfSyncInstrument(uint32_t securityid) {
  return false;
}

bool ChannelController::HasOutOfSyncInstruments() {
  return false;
}

void ChannelController::HandleIncrementalQuoteRequest(Message &m) {

}

