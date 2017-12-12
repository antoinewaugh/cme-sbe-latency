#include "InstrumentController.h"

#include <iostream>

// Match event indicator needs to be exposed - when to commit changes to book
// Also consider commits need to be performed a level out at the channel controller level...

void InstrumentController::switchState(InstrumentState current_state, InstrumentState new_state) {
  state_ = new_state;
  if(new_state == InstrumentState::OUTOFSYNC) {
    channel_->SubscribeToSnapshotsForInstrument(securityid_);
  } else if(new_state == InstrumentState::SYNC || new_state == InstrumentState::DISCONTINUED) {
    channel_->UnsubscribeToSnapshotsForInstrument(securityid_);
  }
}

void InstrumentController::OnSnapshot(SnapshotFullRefresh38 &refresh) {
  auto current_state = state_;
  auto snpt_seqnum = refresh.rptSeq();
  if(current_state == INITIAL) {
    processed_rptseq_ = snpt_seqnum;
    switchState(InstrumentState::INITIAL, InstrumentState::SYNC);
    mdhandler_.OnSnapshot(refresh);
  } else if(current_state == InstrumentState::OUTOFSYNC) {
    if(snpt_seqnum > processed_rptseq_) {
      processed_rptseq_ = snpt_seqnum;
      mdhandler_.Reset();
      switchState(InstrumentState::OUTOFSYNC, InstrumentState::SYNC);
      mdhandler_.OnSnapshot(refresh);
    }
  } else if(current_state == InstrumentState::SYNC && snpt_seqnum > processed_rptseq_){
    std::cout << "WARNING: Snapshot feed is being received faster than incremental - fast forwarding" << '\n';
    processed_rptseq_ = snpt_seqnum;
    mdhandler_.Reset();
    mdhandler_.OnSnapshot(refresh);
  }
}

void InstrumentController::OnSecurityStatus(SecurityStatus30 &status) {
  if((status.securityID() == securityid_) || (status.securityID() == status.securityIDNullValue() && status.securityGroup() == securitygroup_))  {
    
  }
}

void InstrumentController::OnChannelReset() {
  processed_rptseq_ = 0;
  // TODO: clear incremental queue? will automatically occur as seqnum < expected_seqnum
  if(state_ != InstrumentState::DISCONTINUED) {
    switchState(state_, InstrumentState::SYNC);
    mdhandler_.Reset();
  }
}

InstrumentController::InstrumentController(uint32_t securityid, std::string securitygroup, ChannelAccessor* channel ) :
  securityid_(securityid), securitygroup_(securitygroup), state_(InstrumentState::INITIAL), channel_(channel)
{
  channel_->SubscribeToSnapshotsForInstrument(securityid);
}
