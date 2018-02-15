#pragma once

#include <cstdint>
#include "Channel.h"
#include "MarketDataListener.h"

namespace sp {
namespace lltp {
namespace cme {

  class ChannelContext {
  public:
    ChannelContext() = default;
    ChannelContext(Channel* channel);
    void SubscribeToSnapshotsForInstrument(std::uint32_t);
    void UnsubscribeToSnapshotsForInstrument(std::uint32_t);
    void SubscribeInstrumentDownload();
    void UnsubscribeInstrumentDownload();

    void NotifyBookRefresh(u_int64_t id, DepthBook& book);

  private:
      Channel* channel;
  };

}}}
