#pragma once

#include "ChannelAccessor.h"
#include "ChannelInstrument.h"
#include "Packet.h"

#include <map>

class ChannelInstrumentManager {
private:
  std::map<uint32_t,ChannelInstrument> instruments_;
  ChannelAccessor *context_;
  void OnMessage();
  bool CanStopInstrumentFeed();
public:

  ChannelInstrumentManager(ChannelAccessor *);
  void OnInstrumentPacket(Packet *data);
};
