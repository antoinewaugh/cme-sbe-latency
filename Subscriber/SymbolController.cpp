#include "SymbolController.h"

#include <iomanip>
#include <chrono>

namespace sp {
namespace lltp {
namespace cme {

SymbolController::SymbolController(ChannelAccessor *channel) : state_(INIT), channel_(channel) {
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


  // YOU ARE HERE!!!!
  Instrument SymbolController::GetInstrument(std::string const& symbol) {
    auto inst = instruments_.find(symbol);
    if(inst != std::end(instruments_)) {
      return inst->second;
    }
    std::cout << "Unknown instrument " << symbol << '\n';
    return Instrument{};
  }

  std::ostream &operator<<(std::ostream &os, const Instrument &instrument) {
    std::time_t activation_t = instrument.activationdatetime/1000000000;
    std::time_t expiry_t = instrument.expirationdatetime/1000000000;

    os << "securityid: " << instrument.securityid << " symbol: " << instrument.symbol << " securitygroup: "
     << instrument.securitygroup << " type: " << instrument.type << " marketsegmentid: " << instrument.marketsegmentid
       << " activation: " << std::put_time(std::localtime(&activation_t), "%Y-%m-%d %X") << " expiry: " << std::put_time(std::localtime(&expiry_t), "%Y-%m-%d %X");
  return os;
}

}}} // namespace

