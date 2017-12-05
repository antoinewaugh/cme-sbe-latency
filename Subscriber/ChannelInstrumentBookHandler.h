#pragma once

#include "DepthBook.h"
#include "Message.h"

class ChannelInstrumentBookHandler {
public:
  void ApplyIncrementalBidEntry(MDIncrementalRefreshBook32::NoMDEntries &);
  void ApplyIncrementalAskEntry(MDIncrementalRefreshBook32::NoMDEntries &);
  void ApplySnapshotBidEntry(SnapshotFullRefresh38::NoMDEntries &);
  void ApplySnapshotAskEntry(SnapshotFullRefresh38::NoMDEntries &);
  void Clear();
  DepthBook& GetBook();

private:
  DepthBook book_;
};
