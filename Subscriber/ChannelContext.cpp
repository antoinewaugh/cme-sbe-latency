//
// Created by antoine on 4/12/17.
//

#include "ChannelContext.h"

void sp::lltp::cme::ChannelContext::NotifyBookRefresh(u_int64_t id, DepthBook &book) {
    for(auto listener : channel->GetMarketDataListeners()) {
        listener->OnQuote(id, book);
    }
}

sp::lltp::cme::ChannelContext::ChannelContext(sp::lltp::cme::Channel *channel):channel(channel) {}

void sp::lltp::cme::ChannelContext::SubscribeToSnapshotsForInstrument(std::uint32_t securityid) {
    channel->SubscribeToSnapshotsForInstrument(securityid);
}

void sp::lltp::cme::ChannelContext::UnsubscribeToSnapshotsForInstrument(std::uint32_t securityid) {
    channel->SubscribeToSnapshotsForInstrument(securityid);
}

void sp::lltp::cme::ChannelContext::SubscribeInstrumentDownload() {
    channel->SubscribeInstrumentDownload();
}

void sp::lltp::cme::ChannelContext::UnsubscribeInstrumentDownload() {
    channel->UnsubscribeInstrumentDownload();
}

