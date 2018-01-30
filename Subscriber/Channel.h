#pragma once

#include "MulticastReceiver.h"
#include "Connection.h"
#include "Packet.h"
#include "InstrumentMdHandler.h"
#include "ChannelAccessor.h"
#include "ChannelController.h"
#include "SymbolController.h"
#include "Instrument.h"

#include<string>
#include<memory>
#include "Config.h"

namespace sp {
namespace lltp {
namespace cme {

  template<typename T>
  class Channel : public ChannelAccessor {

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

    void OnPacket(Type, Feed, Packet *);

    T handler;
    std::vector<std::string> pendingsubscription_;

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

    void Subscribe(Instrument);

    void Unsubscribe(uint32_t
                     securityid);

    void Subscribe(std::string symbol);

    void Unsubscribe(std::string symbol);

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

    void StartAll();

    void StopAll();

    // exposed via the ChannelAccessor api
    void SubscribeToSnapshotsForInstrument(uint32_t securityid);

    void UnsubscribeToSnapshotsForInstrument(uint32_t securityid);

    void SubscribeInstrumentDownload();

    void UnsubscribeInstrumentDownload();


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

    return std::make_unique<Channel<T>>(handler, channelid, std::move(incrementala), std::move(incrementalb),
                                        std::move(snapshota),
                                        std::move(snapshotb), std::move(instrumenta), std::move(instrumentb));
  }

  template<typename T>
  void Channel<T>::OnPacket(Type type, Feed feed, Packet *data) {
    switch (type) {
      case Type::Instrument:
        symbol_controller_.OnInstrumentPacket(data);
        break;
      case Type::Snapshot:
        channel_controller_.OnSnapshotPacket(data);
        break;
      case Type::Incremental:
        channel_controller_.OnIncrementalPacket(data);
    }
  }

  template<typename T>
  Channel<T>::Channel(T handler, std::string channelid, MulticastReceiver incrementala, MulticastReceiver incrementalb,
                      MulticastReceiver snapshota, MulticastReceiver snapshotb, MulticastReceiver instrumenta,
                      MulticastReceiver instrumentb)
      : channelid_(channelid), incrementala_(std::move(incrementala)), incrementalb_(std::move(incrementalb)),
        snapshota_(std::move(snapshota)), snapshotb_(std::move(snapshotb)),
        instrumenta_(std::move(instrumenta)), instrumentb_(std::move(instrumentb)) // need to provide access to impl
  {
    incrementala_.Register([this](auto type, auto feed, Packet *packet) { this->OnPacket(type, feed, packet); });
    incrementalb_.Register([this](auto type, auto feed, Packet *packet) { this->OnPacket(type, feed, packet); });
    snapshota_.Register([this](auto type, auto feed, Packet *packet) { this->OnPacket(type, feed, packet); });
    snapshotb_.Register([this](auto type, auto feed, Packet *packet) { this->OnPacket(type, feed, packet); });
    instrumenta_.Register([this](auto type, auto feed, Packet *packet) { this->OnPacket(type, feed, packet); });
    instrumentb_.Register([this](auto type, auto feed, Packet *packet) { this->OnPacket(type, feed, packet); });

    channel_controller_ = ChannelController(this);
    symbol_controller_ = SymbolController(this);

  }

  template<typename T>
  std::string Channel<T>::GetId() {
    return channelid_;
  }

  template<typename T>
  void Channel<T>::Subscribe(Instrument inst) {
    channel_controller_.Subscribe(inst);
    StartIncrementalFeed();

  }

  template<typename T>
  void Channel<T>::Unsubscribe(uint32_t securityid) {

    // TODO: remove instruemnts etc.. clean up
    channel_controller_.Unsubscribe(securityid);
  }

  template<typename T>
  void Channel<T>::StartIncrementalA() {
    incrementala_.Join();
  }

  template<typename T>
  void Channel<T>::StopIncrementalB() {
    incrementalb_.Leave();
  }

  template<typename T>
  void Channel<T>::StartSnapshotA() {
    snapshota_.Join();
  }

  template<typename T>
  void Channel<T>::StopSnapshotB() {
    snapshotb_.Leave();
  }

  template<typename T>
  void Channel<T>::StartInstrumentB() {
    instrumentb_.Join();
  }

  template<typename T>
  void Channel<T>::StopInstrumentA() {
    instrumenta_.Leave();
  }

  template<typename T>
  void Channel<T>::StartIncrementalFeed() {
    std::cout << "Starting Incremental Feeds: " << channelid_ << '\n';
    StartIncrementalA();
    StartIncrementalB();
  }

  template<typename T>
  void Channel<T>::StopIncrementalFeed() {
    std::cout << "Stopping Incremental Feeds: " << channelid_ << '\n';
    StopIncrementalA();
    StopIncrementalB();
  }

  template<typename T>
  void Channel<T>::StartSnapshotFeed() {
    std::cout << "Starting Snapshot Feeds: " << channelid_ << '\n';
    StartSnapshotA();
    StartSnapshotB();
  }

  template<typename T>
  void Channel<T>::StopSnapshotFeed() {
    std::cout << "Stopping Snapshot Feeds: " << channelid_ << '\n';
    StopSnapshotA();
    StopSnapshotB();
  }

  template<typename T>
  void Channel<T>::StartInstrumentFeed() {
    std::cout << "Starting Instrument Feeds: " << channelid_ << '\n';
    StartInstrumentA();
    StartInstrumentB();
  }

  template<typename T>
  void Channel<T>::StopInstrumentFeed() {
    std::cout << "Stopping Instrument Feeds: " << channelid_ << '\n';
    StopInstrumentA();
    StopInstrumentB();
    for (auto &symbol : pendingsubscription_) {
      auto inst = symbol_controller_.GetInstrument(symbol);
      Subscribe(inst);
    }
    pendingsubscription_.clear();
  }

  template<typename T>
  void Channel<T>::StartAll() {
    StartIncrementalFeed();
    StartSnapshotFeed();
    StartInstrumentFeed();
  }

  template<typename T>
  void Channel<T>::StopAll() {
    StopIncrementalFeed();
    StopSnapshotFeed();
    StopInstrumentFeed();
  }

  template<typename T>
  void Channel<T>::StopIncrementalA() {
    incrementala_.Leave();
  }

  template<typename T>
  void Channel<T>::StartIncrementalB() {
    incrementalb_.Join();
  }

  template<typename T>
  void Channel<T>::StartInstrumentA() {
    instrumenta_.Join();
  }

  template<typename T>
  void Channel<T>::StopInstrumentB() {
    instrumentb_.Leave();
  }

  template<typename T>
  void Channel<T>::StopSnapshotA() {
    snapshota_.Leave();
  }

  template<typename T>
  void Channel<T>::StartSnapshotB() {
    snapshotb_.Join();
  }

  template<typename T>
  void Channel<T>::SubscribeToSnapshotsForInstrument(uint32_t securityid) {
    channel_controller_.AddOutOfSyncInstrument(securityid);
    if (!IsSnapshotFeedActive()) {
      StartSnapshotFeed();
    }
  }

  template<typename T>
  bool Channel<T>::IsSnapshotFeedActive() {
    return snapshota_.IsActive() || snapshotb_.IsActive();
  }

  template<typename T>
  void Channel<T>::UnsubscribeToSnapshotsForInstrument(uint32_t securityid) {
    std::cout << "Unsubscribing to snapshots for : " << securityid << '\n';
    if (channel_controller_.RemoveOutOfSyncInstrument(securityid)) {
      if (IsSnapshotFeedActive()) {
        if (!channel_controller_.HasOutOfSyncInstruments()) {
          StopSnapshotFeed();
        }
      }
    }
  }

  template<typename T>
  void Channel<T>::UnsubscribeInstrumentDownload() {
    StopInstrumentFeed();
  }

  template<typename T>
  void Channel<T>::SubscribeInstrumentDownload() {
    StartInstrumentFeed();
  }

  template<typename T>
  void Channel<T>::Subscribe(std::string symbol) {
    pendingsubscription_.push_back(symbol);
  }

  template<typename T>
  void Channel<T>::Unsubscribe(std::string symbol) {
    symbol_controller_.GetInstrument(symbol).securityid;
  }
}}}
