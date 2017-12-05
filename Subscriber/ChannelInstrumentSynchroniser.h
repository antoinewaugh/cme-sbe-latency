#pragma once

#include "Message.h"
#include "InstrumentController.h"

/*
 * Monitors sequence numbers, redundant packets,
 * and gaps, triggering Channel to Join/Leave Recovery streams.
 *
 * Valid messages are passed onto the InstrumentController
 * to build state.
 */

class ChannelInstrumentSynchroniser {

public:

  void OnSnapshotRefresh(Message& );
  void OnIncrementalRefresh(Message &);

private:

  InstrumentController mdhandler_;
  uint64_t seqnum_;
  uint64_t rptseq_;

};
