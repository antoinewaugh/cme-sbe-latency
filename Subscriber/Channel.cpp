#include "Channel.h"

void Channel::OnPacket(Type type, Feed feed, Packet *data) {
  switch(type) {
//    case Instrument: instruments_.OnInstrumentPacket(data); break;
    case Snapshot: depthbooks_.OnSnapshotPacket(data); break;
    case Incremental: depthbooks_.OnIncrementalPacket(data); break;
  }
}
