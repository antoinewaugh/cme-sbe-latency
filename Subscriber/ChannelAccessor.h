#pragma once

#include <cstdint>

class ChannelAccessor {
public:
  virtual void SubscribeToSnapshotsForInstrument(std::uint32_t) = 0;
  virtual void UnsubscribeToSnapshotsForInstrument(std::uint32_t) = 0;
};

