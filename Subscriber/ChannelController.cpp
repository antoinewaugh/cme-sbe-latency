#include "ChannelController.h"
#include <algorithm>

#include <iostream>

#include <chrono>
void PrintTime(std::string ref, unsigned long transacttime) {
  unsigned long our_ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::system_clock::now().time_since_epoch())
      .count();
  int diff = our_ts - transacttime;

  std::cout << ref << ": " << diff << " nanos" <<  '\n';
}

void ChannelController::HandleSnapshotMessage(Message& m) {
  auto& snapshot = m.Get<SnapshotFullRefresh38>();
  auto inst_controller = GetInstrumentController(snapshot.securityID());
  if(inst_controller) {
    inst_controller->OnSnapshot(snapshot, snapshot.transactTime());
  }
}

InstrumentController* ChannelController::GetInstrumentController(uint32_t securityid) {
  auto search = instrument_controllers_.find(securityid);
  if(search != std::end(instrument_controllers_)){
    return &(search->second);
  }
  return nullptr;
}

void ChannelController::HandleIncrementalSecurityStatus(Message& m) {
//  void ChannelController::HandleIncrementalSecurityStatus(Message& m) {
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
  auto seqnum = packet->GetSeqNum();
  if(seqnum == expectedseqnum_ || expectedseqnum_ == 0) {
    expectedseqnum_ = seqnum + 1;
    while (packet->HasNextMessage()) {
      auto &message = packet->NextMessage();
      OnIncrementalMessage(message);
    }
  } else if (seqnum > expectedseqnum_) {
    // seq gap detected on incremental - currently only reset occurs when
    std::cout << "Packet level seq gap detected on incremental channel, ignoring unless RptSeq sync issue." << '\n';
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
    instrument_controllers_.emplace(securityid, std::move(InstrumentController(securityid, std::string(), channel_)));
  }
}

void ChannelController::Unsubscribe(uint32_t securityid) {
// TODO
}

ChannelController::ChannelController(ChannelAccessor *channel):channel_(channel) {

}

void ChannelController::AddOutOfSyncInstrument(std::uint32_t securityid) {
  auto found = std::find(std::begin(outofsync_instruments_), std::end(outofsync_instruments_), securityid);
  if (found == std::end(outofsync_instruments_)) {
   outofsync_instruments_.push_back(securityid);
  }
}

bool ChannelController::RemoveOutOfSyncInstrument(uint32_t securityid) {
  auto found = std::find(std::begin(outofsync_instruments_), std::end(outofsync_instruments_), securityid);
  if (found != std::end(outofsync_instruments_)) {
    outofsync_instruments_.erase(found);
    return true;
  }
  return false;
}

bool ChannelController::HasOutOfSyncInstruments() {
  return outofsync_instruments_.size() > 0;
}

void ChannelController::HandleIncrementalQuoteRequest(Message &m) {

}

void ChannelController::Commit() {
  for(auto && [_, inst_controller] : instrument_controllers_) {
     inst_controller.Commit();
  }
}


