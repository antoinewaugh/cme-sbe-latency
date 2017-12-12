#include "Channel.h"
#include "Config.h"
/*
ChannelNetworkController {

};
namespace sortino {
namespace connectivity {
namespace cme {

 // subscriber acts as the builder,
  class Subscriber {
    Channel std::unique_ptr<Channel>
    channel;

  };

}}} // sortino.connectivity.cme
*/
void Channel::OnPacket(Type type, Feed feed, Packet *data) {
  switch(type) {
//    case Instrument: instruments_.OnInstrumentPacket(data); break;
    case Snapshot: channel_controller_.OnSnapshotPacket(data); break;
    case Incremental: channel_controller_.OnIncrementalPacket(data); break;
  }
}

Channel::Channel(std::string channelid, MulticastReceiver incrementala, MulticastReceiver incrementalb,
                 MulticastReceiver snapshota, MulticastReceiver snapshotb, MulticastReceiver instrumenta,
                 MulticastReceiver instrumentb)
    : channelid_(channelid), incrementala_(std::move(incrementala)), incrementalb_(std::move(incrementalb)),
      snapshota_(std::move(snapshota)), snapshotb_(std::move(snapshotb)),
      instrumenta_(std::move(instrumenta)), instrumentb_(std::move(instrumentb)) // need to provide access to impl
{
  incrementala_.Register([this](auto type, auto feed, Packet* packet){this->OnPacket(type, feed, packet);});
  incrementalb_.Register([this](auto type, auto feed, Packet* packet){this->OnPacket(type, feed, packet);});
  snapshota_.Register([this](auto type, auto feed, Packet* packet){this->OnPacket(type, feed, packet);});
  snapshotb_.Register([this](auto type, auto feed, Packet* packet){this->OnPacket(type, feed, packet);});
  instrumenta_.Register([this](auto type, auto feed, Packet* packet){this->OnPacket(type, feed, packet);});
  instrumentb_.Register([this](auto type, auto feed, Packet* packet){this->OnPacket(type, feed, packet);});

  channel_controller_ = ChannelController(this);
}

std::string Channel::GetId() {
  return channelid_;
}

void Channel::Subscribe(uint32_t securityid) {
  channel_controller_.Subscribe(securityid);
  StartIncrementalFeed();
  StartSnapshotFeed();
}

void Channel::Unsubscribe(uint32_t securityid) {
  channel_controller_.Unsubscribe(securityid);
}

void Channel::StartIncrementalA() {
  incrementala_.Join();
}

void Channel::StopIncrementalB() {
  incrementalb_.Leave();
}

void Channel::StartSnapshotA() {
  snapshota_.Join();
}

void Channel::StopSnapshotB() {
  snapshotb_.Leave();
}

void Channel::StartInstrumentB() {
  instrumentb_.Join();
}

void Channel::StopInstrumentA() {
  instrumenta_.Leave();
}

void Channel::StartIncrementalFeed() {
  StartIncrementalA();
  StartIncrementalB();
}

void Channel::StopIncrementalFeed() {
  StopIncrementalA();
  StopIncrementalB();
}

void Channel::StartSnapshotFeed() {
  StartSnapshotA();
  StartSnapshotB();
}

void Channel::StopSnapshotFeed() {
  StopSnapshotA();
  StopSnapshotB();
}

void Channel::StartInstrumentFeed() {
  StartInstrumentA();
  StartInstrumentB();
}

void Channel::StopInstrumentFeed() {
  StopInstrumentA();
  StopInstrumentB();
}

void Channel::StartAll() {
  StartIncrementalFeed();
  StartSnapshotFeed();
  StartInstrumentFeed();
}

void Channel::StopAll() {
  StopIncrementalFeed();
  StopSnapshotFeed();
  StopInstrumentFeed();
}

void Channel::StopIncrementalA() {
  incrementala_.Leave();
}

void Channel::StartIncrementalB() {
  incrementalb_.Join();
}

void Channel::StartInstrumentA() {
  instrumenta_.Join();
}

void Channel::StopInstrumentB() {
  instrumentb_.Leave();
}

void Channel::StopSnapshotA() {
  snapshota_.Leave();
}

void Channel::StartSnapshotB() {
  snapshotb_.Join();
}

void Channel::SubscribeToSnapshotsForInstrument(uint32_t securityid) {
  channel_controller_.AddOutOfSyncInstrument(securityid);
}

bool Channel::IsSnapshotFeedActive() {
  return snapshota_.IsActive() || snapshota_.IsActive();
}

void Channel::UnsubscribeToSnapshotsForInstrument(uint32_t securityid) {
  if(channel_controller_.RemoveOutOfSyncInstrument(securityid)) {
    if(IsSnapshotFeedActive()) {
      if (!channel_controller_.HasOutOfSyncInstruments()) {
        StopSnapshotFeed();
      }
    }
  }
}


std::unique_ptr<Channel> Channel::make_channel(std::string channelid, Config &config,
                                       boost::asio::io_service &io_service,
                                       boost::asio::ip::address &listen_address
                                       ) {

  auto incrementala =
      std::move(MulticastReceiver(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Incremental, Feed::A)));

  auto incrementalb = std::move(MulticastReceiver(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Incremental, Feed::B)));

  auto snapshota =
      std::move(MulticastReceiver(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Snapshot, Feed::A)));

  auto snapshotb =
      std::move(MulticastReceiver(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Snapshot, Feed::B)));

  auto instrumenta =
      std::move(MulticastReceiver(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Instrument, Feed::A)));

auto instrumentb = std::move(MulticastReceiver(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Instrument, Feed::B)));

  return std::make_unique<Channel>(Channel(channelid, std::move(incrementala), std::move(incrementalb), std::move(snapshota),
      std::move(snapshotb), std::move(instrumenta), std::move(instrumentb)));
}