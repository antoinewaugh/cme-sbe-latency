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
//void OnIncrementalRefreshBook(MDIncrementalRefreshBook32&);
  void OnIncrementalRefreshBookEntry(MDIncrementalRefreshBook32::NoMDEntries &);
  void OnIncrementalRefreshTrade(MDIncrementalRefreshTrade36&);
  void OnIncrementalRefreshVolume(MDIncrementalRefreshVolume37&);
  void OnIncrementalTradeSumary(MDIncrementalRefreshTradeSummary42& );
  void OnIncrementalDailyStatistics(MDIncrementalRefreshDailyStatistics33&);
  void OnIncrementalSessionStatistics(MDIncrementalRefreshSessionStatistics35&);
  void OnIncrementalLimitsBanding(MDIncrementalRefreshLimitsBanding34&);
  void OnSecurityStatus(MDIncrementalRefreshLimitsBanding34&);
  void OnChannelReset();

private:
  int securityid_;
  InstrumentState state_;
  long processed_rptseq_;
  std::string securitydesc_;

  ChannelAccessor channel_; //
  InstrumentMdHandler mdhandler_;

  void switchState(InstrumentState , InstrumentState );
};

