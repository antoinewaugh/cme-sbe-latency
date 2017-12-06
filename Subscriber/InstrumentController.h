#pragma once

#include "InstrumentMdHandler.h"
#include "ChannelAccessor.h"

/*
 * Manages seqnum and rtpseq values for a specific instrument.
 * (Un)Subscribes to instrument feeds when recovery/gap detected.
 *
 * Valid messages are passed onto InstrumentMDHandler
 */

enum InstrumentState {NEW, INITIAL, SYNC, OUTOFSYNC, DISCONTINUED};

class InstrumentController {

public:
  InstrumentController();
  void OnSnapshot(SnapshotFullRefresh38&);
  template<typename T>
  void OnIncremental(T &entry);
  void OnSecurityStatus(SecurityStatus30&); // TODO: need to support logic from ChannelController if securityID blank send to all
  void OnChannelReset();

private:
  uint32_t securityid_;
  std::string securitygroup_;
  InstrumentState state_;
  long processed_rptseq_;
  std::string securitydesc_;

  ChannelAccessor channel_; //
  InstrumentMdHandler mdhandler_;

  void switchState(InstrumentState , InstrumentState );
};

