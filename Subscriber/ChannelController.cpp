#include "ChannelController.h"

void ChannelController::HandleSnapshotMessage(Message& m) {
  auto snapshot = m.Get<SnapshotFullRefresh38>();
  auto inst_controller = GetInstrumentController(snapshot.securityID());
  if(inst_controller) {
    inst_controller->OnSnapshot(snapshot);
  }
}

InstrumentController* ChannelController::GetInstrumentController(uint32_t securityid) {
  auto search = instrument_controllers_.find(securityid);
  if(search != instrument_controllers_.end()) {
    return &(search->second);
  }
  return nullptr;
}

template<typename T>
void ChannelController::HandleIncrementalMessage(Message& m) {
  auto message = m.Get<T>();
  auto& entry = message.noMDEntries();
  while(entry.hasNext()) {
    entry.next();
    auto inst_controller = GetInstrumentController(entry.securityID());
    if(inst_controller) {
      inst_controller->OnIncremental(entry);
    }
  }
}

void ChannelController::OnIncrementalMessage(Message& m) {
  auto templateid = m.GetTemplateId();
  if(templateid == MDIncrementalRefreshBook32::sbeTemplateId()) {
    HandleIncrementalMessage<MDIncrementalRefreshBook32>(m);
  } else if(templateid == MDIncrementalRefreshTrade36::sbeTemplateId()) {
    HandleIncrementalMessage<MDIncrementalRefreshTrade36>(m);
  } else if(templateid == MDIncrementalRefreshTradeSummary42::sbeTemplateId()) {
    HandleIncrementalMessage<MDIncrementalRefreshTradeSummary42>(m);
  }
}

void ChannelController::OnIncrementalPacket(Packet *packet) {
  while(packet->HasNext()) {
    auto message = packet->Next();
    OnIncrementalMessage(*message);
  }
}

void ChannelController::OnSnapshotPacket(Packet *packet) {
  while(packet->HasNext()) {
    auto message = packet->Next();
    HandleSnapshotMessage(*message);
  }
}

