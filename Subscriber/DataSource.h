#pragma once

#include "Connection.h"
#include "MulticastReceiver.h"

#include <functional>
#include <ostream>

enum NetworkPacketStatus {Redundant, Expected, RecoveryRequired};

struct DataSource {
  MulticastReceiver primary, secondary;
  DataSource(MulticastReceiver primary, MulticastReceiver secondary);
  void Join();
  void Leave();
  void Reset();
  void RegisterCallback(
      std::function<void(char *, size_t, NetworkPacketStatus)> callback);

private:
  void OnData(char *, size_t);
  NetworkPacketStatus GetNetworkPacketStatus(uint32_t seqnum);
  std::function<void(char *, size_t, NetworkPacketStatus)> callback_;
  uint64_t lastseqnum_ = 0;
  friend std::ostream &operator<<(std::ostream &os, const DataSource &source);
};
