#pragma once

#include "MulticastReceiver.h"
#include "Connection.h"
#include "Packet.h"
#include "InstrumentMdHandler.h"
#include "ChannelAccessor.h"
#include "ChannelController.h"
#include "SymbolController.h"

#include<string>
#include<memory>
#include "Config.h"

using sp::lltp::cme::SymbolController;

template<typename T>
class Channel: public ChannelAccessor {

private:

  std::string channelid_;
  MulticastReceiver incrementala_;
  MulticastReceiver incrementalb_;
  MulticastReceiver snapshota_;
  MulticastReceiver snapshotb_;
  MulticastReceiver instrumenta_;
  MulticastReceiver instrumentb_;

  SymbolController symbol_controller_;
  ChannelController channel_controller_;
  bool IsSnapshotFeedActive();
  void OnPacket(Type, Feed, Packet*);

  T handler;

public:
  Channel(T handler, std::string channelid,
  MulticastReceiver incrementala,
  MulticastReceiver incrementalb,
  MulticastReceiver snapshota,
  MulticastReceiver snapshotb,
  MulticastReceiver instrumenta,
  MulticastReceiver instrumentb);

  Channel(Channel &&) = delete;
  std::string GetId();

  void Subscribe(uint32_t securityid);
  void Unsubscribe(uint32_t
                   securityid);

  void StartIncrementalA();
  void StopIncrementalA();
  void StartIncrementalB();
  void StopIncrementalB();
  void StartSnapshotA();
  void StopSnapshotA();
  void StartSnapshotB();
  void StopSnapshotB();
  void StartInstrumentA();
  void StopInstrumentA();
  void StartInstrumentB();
  void StopInstrumentB();
  void StartIncrementalFeed();
  void StopIncrementalFeed();
  void StartSnapshotFeed();
  void StopSnapshotFeed();
  void StartInstrumentFeed();
  void StopInstrumentFeed();

  void SubscribeToSnapshotsForInstrument(uint32_t securityid);
  void UnsubscribeToSnapshotsForInstrument(uint32_t securityid);
  void UnsubscribeInstrumentDownload();

  void StartAll();
  void StopAll();

  static std::unique_ptr<Channel<T>> make_channel(T handler,
                                   std::string channelid, Config &config,
                                   boost::asio::io_service &io_service,
                                   boost::asio::ip::address &listen_address
  );
};


template<typename T>
std::unique_ptr<Channel<T>> Channel<T>::make_channel(T handler, std::string channelid, Config &config,
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

  return std::make_unique<Channel<T>>(handler, channelid, std::move(incrementala), std::move(incrementalb), std::move(snapshota),
                                      std::move(snapshotb), std::move(instrumenta), std::move(instrumentb));
}

template<typename T> void Channel<T>::OnPacket(Type type, Feed feed, Packet *data) {
  std::cout << "type: " << type << ", feed: " << feed << " , Packet seqnum " << data->GetSeqNum() << '\n';
  return;

  switch(type) {
//    case Instrument: symbol_controller_.OnInstrumentPacket(data); break;
    case Snapshot: channel_controller_.OnSnapshotPacket(data); break;
    case Incremental: channel_controller_.OnIncrementalPacket(data); break;
  }
}

template<typename T> Channel<T>::Channel(T handler, std::string channelid, MulticastReceiver incrementala, MulticastReceiver incrementalb,
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

  StartInstrumentFeed();

}

template<typename T> std::string Channel<T>::GetId() {
  return channelid_;
}

template<typename T> void Channel<T>::Subscribe(uint32_t securityid) {
  channel_controller_.Subscribe(securityid);
  StartIncrementalFeed();
}

template<typename T> void Channel<T>::Unsubscribe(uint32_t securityid) {

  // TODO: remove instruemnts etc.. clean up
  channel_controller_.Unsubscribe(securityid);
}

template<typename T> void Channel<T>::StartIncrementalA() {
  incrementala_.Join();
}

template<typename T> void Channel<T>::StopIncrementalB() {
  incrementalb_.Leave();
}

template<typename T> void Channel<T>::StartSnapshotA() {
  snapshota_.Join();
}

template<typename T> void Channel<T>::StopSnapshotB() {
  snapshotb_.Leave();
}

template<typename T> void Channel<T>::StartInstrumentB() {
  instrumentb_.Join();
}

template<typename T> void Channel<T>::StopInstrumentA() {
  instrumenta_.Leave();
}

template<typename T> void Channel<T>::StartIncrementalFeed() {
  std::cout << "Starting Incremental Feeds: " << channelid_ << '\n';
  StartIncrementalA();
  StartIncrementalB();
}

template<typename T> void Channel<T>::StopIncrementalFeed() {
  std::cout << "Stopping Incremental Feeds: " << channelid_ << '\n';
  StopIncrementalA();
  StopIncrementalB();
}

template<typename T> void Channel<T>::StartSnapshotFeed() {
  std::cout << "Starting Snapshot Feeds: " << channelid_ << '\n';
  StartSnapshotA();
  StartSnapshotB();
}

template<typename T> void Channel<T>::StopSnapshotFeed() {
  std::cout << "Stopping Snapshot Feeds: " << channelid_ << '\n';
  StopSnapshotA();
  StopSnapshotB();
}

template<typename T> void Channel<T>::StartInstrumentFeed() {
  std::cout << "Starting Instrument Feeds: " << channelid_ << '\n';
  StartInstrumentA();
  StartInstrumentB();
}

template<typename T> void Channel<T>::StopInstrumentFeed() {
  std::cout << "Stopping Instrument Feeds: " << channelid_ << '\n';
  StopInstrumentA();
  StopInstrumentB();
}

template<typename T> void Channel<T>::StartAll() {
  StartIncrementalFeed();
  StartSnapshotFeed();
  StartInstrumentFeed();
}

template<typename T> void Channel<T>::StopAll() {
  StopIncrementalFeed();
  StopSnapshotFeed();
  StopInstrumentFeed();
}

template<typename T> void Channel<T>::StopIncrementalA() {
  incrementala_.Leave();
}

template<typename T> void Channel<T>::StartIncrementalB() {
  incrementalb_.Join();
}

template<typename T> void Channel<T>::StartInstrumentA() {
  instrumenta_.Join();
}

template<typename T> void Channel<T>::StopInstrumentB() {
  instrumentb_.Leave();
}

template<typename T> void Channel<T>::StopSnapshotA() {
  snapshota_.Leave();
}

template<typename T> void Channel<T>::StartSnapshotB() {
  snapshotb_.Join();
}

template<typename T> void Channel<T>::SubscribeToSnapshotsForInstrument(uint32_t securityid) {
  channel_controller_.AddOutOfSyncInstrument(securityid);
  if(!IsSnapshotFeedActive()) {
    StartSnapshotFeed();
  }
}

template<typename T> bool Channel<T>::IsSnapshotFeedActive() {
  return snapshota_.IsActive() || snapshotb_.IsActive();
}

template<typename T> void Channel<T>::UnsubscribeToSnapshotsForInstrument(uint32_t securityid) {
  std::cout << "Unsubscribing to snapshots for : " << securityid << '\n';
  if(channel_controller_.RemoveOutOfSyncInstrument(securityid)) {
    if(IsSnapshotFeedActive()) {
      if (!channel_controller_.HasOutOfSyncInstruments()) {
        StopSnapshotFeed();
      }
    }
  }
}

template <typename T>
void Channel<T>::UnsubscribeInstrumentDownload() {
  StopInstrumentFeed();
}



