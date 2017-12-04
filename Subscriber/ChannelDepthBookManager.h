#pragma once

#include "Packet.h"

class ChannelDepthBookManager {
  void OnSnapshotPacket(Packet *data);
  void OnIncrementalPacket(Packet *data);
};

