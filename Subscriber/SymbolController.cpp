#include "SymbolController.h"

namespace sp {
namespace lltp {
namespace cme {

SymbolController::SymbolController(ChannelContext *channel) : state_(INIT), channel_(channel) {
  channel_->SubscribeInstrumentDownload();
  ResetState();
}

void SymbolController::ResetState() {
  expectedseqnum_ = 1;
  totalnumreports_ = 0;
  processedreports_ = 0;
  instruments_.clear();
}

void SymbolController::SwitchState(SymbolController::DownloadState oldstate, SymbolController::DownloadState newstate) {
  state_ = newstate;
  if(newstate == DOWNLOADED) {
    channel_->UnsubscribeInstrumentDownload();
  }
}

void SymbolController::HandlePacket(Packet* packet) {
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

/*
 * Test Cases:
 * INIT
 *  - receive seqnum 2/7, we will wait until seqnum == 1
 *  - recevie seqnum 1, process packet and set total reports
 *  DOWNLOADINT
 *  - receive seqnum 3/7, expected == 2, we will reset expected == 1, num reports = 0, set state to INIT
 */

void SymbolController::OnInstrumentPacket(Packet *packet) {
  auto seqnum = packet->GetSeqNum();
  if(seqnum == expectedseqnum_ && state_ != DOWNLOADED) {
    HandlePacket(packet);
    if(state_ == INIT) HandlePacket(packet); // must be below handle packet
    expectedseqnum_++;
  } else if(seqnum > expectedseqnum_) {
    if(state_ == DOWNLOADING) {
     ResetState();
     SwitchState(DOWNLOADING, INIT);
    }
  }
}


  Instrument SymbolController::GetInstrument(std::string const& symbol) {
    auto inst = instruments_.find(symbol);
    if(inst != std::end(instruments_)) {
      return inst->second;
    }
    std::cout << "Unknown instrument " << symbol << '\n';
    return Instrument{};
  }


}}} // namespace

