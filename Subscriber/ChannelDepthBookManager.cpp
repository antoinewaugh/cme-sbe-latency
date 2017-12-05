#include "ChannelDepthBookManager.h"

#include "Message.h"

void ChannelDepthBookManager::OnSnapshotPacket(Packet *data) {


}

void ChannelDepthBookManager::OnIncrementalPacket(Packet *data) {
  while(data.HasNext()) {
    auto message = data.Next(); // return iterator, ensure no copy
    auto type = message.GetType();

    switch(type) {
      case MDIncrementalRefreshOrderBook43:
        handleIncrementalRefreshOrderBook(message.getIncremetnalRefreshOrderBook()); break;
      case MDIncrementalRefreshTradeSummary42:
        handleIncrementalRefreshTradeSummary(message); break;
      case MDIncrementalRefreshOrderBook43: handleIncrementalRefreshOrderBook(); break;
      case MDIncrementalRefreshOrderBook43: handleIncrementalRefreshOrderBook(); break;
    }
  }
}

void handleIncrementalRefreshOrderBook(uint64_t seqnum, MDIncrementalRefreshOrderBook43 message) {
  auto &entry = message.noMDEntries();
  while (entry.hasNext()) {
    entry.next();

    if (entry.mDEntryType() == MDEntryType::BookReset) {
      handleChannelReset()
    } else {
      auto securityid = entry.securityID();
      auto instrument = instruments_.find(securityid);
      if(instrument != instruments_.end()) {
        updateSecurityFromIncrementalRefresh(entry, seqnum, securityid);
      }
    }
  }
}