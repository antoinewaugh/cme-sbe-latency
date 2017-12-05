#pragma once

#include <cstdint>

class ChannelAccessor {
public:
  void StartInstrumentFeed();
  void StopInstrumentFeed();
  void SubscribeToSnapshotsForInstrument(std::uint64_t);
  void UnsubscribeToSnapshotsForInstrument(std::uint64_t);
};

