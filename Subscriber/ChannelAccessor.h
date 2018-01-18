#pragma once

#include <cstdint>

class ChannelAccessor {
public:
  virtual void SubscribeToSnapshotsForInstrument(std::uint32_t) = 0;
  virtual void UnsubscribeToSnapshotsForInstrument(std::uint32_t) = 0;
  virtual void SubscribeInstrumentDownload() = 0;
  virtual void UnsubscribeInstrumentDownload() = 0;
//  virtual void SubscribeToInstruments(std::uint32_t) = 0;
//  virtual void UnsubscribeToInstruments(std::uint32_t) = 0;
//  virtual void AddInstrument(std::uint32_t) = 0;
//  virtual T& GetHandler() = 0;
};

