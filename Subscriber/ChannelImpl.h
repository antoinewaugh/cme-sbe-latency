#pragma once

#include "MulticastReceiver.h"
#include "Connection.h"
#include "Packet.h"
#include "InstrumentMdHandler.h"
#include "ChannelContext.h"
#include "ChannelController.h"
#include "SymbolController.h"
#include "Instrument.h"
#include "MarketDataListener.h"

#include<string>
#include<algorithm>
#include<memory>
#include "Config.h"

namespace sp {
namespace lltp {
namespace cme {

  class ChannelImpl : public Channel {

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
    ChannelContext channel_context_;

    bool IsSnapshotFeedActive();

    void OnPacket(Type, Feed, Packet *);

    std::vector<MarketDataListener*> mdlisteners;
    std::vector<std::string> pendingsubscription_;

  public:
    ChannelImpl(std::string channelid,
            MulticastReceiver incrementala,
            MulticastReceiver incrementalb,
            MulticastReceiver snapshota,
            MulticastReceiver snapshotb,
            MulticastReceiver instrumenta,
            MulticastReceiver instrumentb);

    ChannelImpl(ChannelImpl &&) = delete;

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

    // exposed via the ChannelContext api
    void SubscribeToSnapshotsForInstrument(uint32_t securityid);

    void UnsubscribeToSnapshotsForInstrument(uint32_t securityid);

    void SubscribeInstrumentDownload();

    void UnsubscribeInstrumentDownload();

    void RegisterMarketDataListener(MarketDataListener *listener) {
      mdlisteners.push_back(listener);
    }

    void RemoveMarketDataListener(MarketDataListener *listener) {
      auto it = std::find(std::begin(mdlisteners), std::end(mdlisteners), listener);
      mdlisteners.erase(it);
    }

    std::vector<MarketDataListener*> GetMarketDataListeners() {
      return mdlisteners;
    }

    static std::unique_ptr<ChannelImpl> make_channel(
                                                    std::string channelid, Config &config,
                                                    boost::asio::io_service &io_service,
                                                    boost::asio::ip::address &listen_address
    );
  };


  std::unique_ptr<ChannelImpl> ChannelImpl::make_channel(std::string channelid, Config &config,
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

    return std::make_unique<ChannelImpl>(channelid, std::move(incrementala), std::move(incrementalb),
                                        std::move(snapshota),
                                        std::move(snapshotb), std::move(instrumenta), std::move(instrumentb));
  }

  void ChannelImpl::OnPacket(Type type, Feed feed, Packet *data) {
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

  ChannelImpl::ChannelImpl(std::string channelid, MulticastReceiver incrementala, MulticastReceiver incrementalb,
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

    channel_context_ = ChannelContext(this);
    channel_controller_ = ChannelController(&channel_context_);
    symbol_controller_ = SymbolController(&channel_context_);

  }

  std::string ChannelImpl::GetId() {
    return channelid_;
  }

  void ChannelImpl::Subscribe(Instrument inst) {
    channel_controller_.Subscribe(inst);
    StartIncrementalFeed();

  }

  void ChannelImpl::Unsubscribe(uint32_t securityid) {

    // TODO: remove instruemnts etc.. clean up
    channel_controller_.Unsubscribe(securityid);
  }

  void ChannelImpl::StartIncrementalA() {
    incrementala_.Join();
  }

  void ChannelImpl::StopIncrementalB() {
    incrementalb_.Leave();
  }

  void ChannelImpl::StartSnapshotA() {
    snapshota_.Join();
  }

  void ChannelImpl::StopSnapshotB() {
    snapshotb_.Leave();
  }

  void ChannelImpl::StartInstrumentB() {
    instrumentb_.Join();
  }

  void ChannelImpl::StopInstrumentA() {
    instrumenta_.Leave();
  }

  void ChannelImpl::StartIncrementalFeed() {
    std::cout << "Starting Incremental Feeds: " << channelid_ << '\n';
    StartIncrementalA();
    StartIncrementalB();
  }

  void ChannelImpl::StopIncrementalFeed() {
    std::cout << "Stopping Incremental Feeds: " << channelid_ << '\n';
    StopIncrementalA();
    StopIncrementalB();
  }

  void ChannelImpl::StartSnapshotFeed() {
    std::cout << "Starting Snapshot Feeds: " << channelid_ << '\n';
    StartSnapshotA();
    StartSnapshotB();
  }

  void ChannelImpl::StopSnapshotFeed() {
    std::cout << "Stopping Snapshot Feeds: " << channelid_ << '\n';
    StopSnapshotA();
    StopSnapshotB();
  }

  void ChannelImpl::StartInstrumentFeed() {
    std::cout << "Starting Instrument Feeds: " << channelid_ << '\n';
    StartInstrumentA();
    StartInstrumentB();
  }

  void ChannelImpl::StopInstrumentFeed() {
    std::cout << "Stopping Instrument Feeds: " << channelid_ << '\n';
    StopInstrumentA();
    StopInstrumentB();
    for (auto &symbol : pendingsubscription_) {
      auto inst = symbol_controller_.GetInstrument(symbol);
      Subscribe(inst);
    }
    pendingsubscription_.clear();
  }

  void ChannelImpl::StartAll() {
    StartIncrementalFeed();
    StartSnapshotFeed();
    StartInstrumentFeed();
  }

  void ChannelImpl::StopAll() {
    StopIncrementalFeed();
    StopSnapshotFeed();
    StopInstrumentFeed();
  }

  void ChannelImpl::StopIncrementalA() {
    incrementala_.Leave();
  }

  void ChannelImpl::StartIncrementalB() {
    incrementalb_.Join();
  }

  void ChannelImpl::StartInstrumentA() {
    instrumenta_.Join();
  }

  void ChannelImpl::StopInstrumentB() {
    instrumentb_.Leave();
  }

  void ChannelImpl::StopSnapshotA() {
    snapshota_.Leave();
  }

  void ChannelImpl::StartSnapshotB() {
    snapshotb_.Join();
  }

  void ChannelImpl::SubscribeToSnapshotsForInstrument(uint32_t securityid) {
    channel_controller_.AddOutOfSyncInstrument(securityid);
    if (!IsSnapshotFeedActive()) {
      StartSnapshotFeed();
    }
  }

  bool ChannelImpl::IsSnapshotFeedActive() {
    return snapshota_.IsActive() || snapshotb_.IsActive();
  }

  void ChannelImpl::UnsubscribeToSnapshotsForInstrument(uint32_t securityid) {
    std::cout << "Unsubscribing to snapshots for : " << securityid << '\n';
    if (channel_controller_.RemoveOutOfSyncInstrument(securityid)) {
      if (IsSnapshotFeedActive()) {
        if (!channel_controller_.HasOutOfSyncInstruments()) {
          StopSnapshotFeed();
        }
      }
    }
  }

  void ChannelImpl::UnsubscribeInstrumentDownload() {
    StopInstrumentFeed();
  }

  void ChannelImpl::SubscribeInstrumentDownload() {
    StartInstrumentFeed();
  }

  void ChannelImpl::Subscribe(std::string symbol) {
    pendingsubscription_.push_back(symbol);
  }

  void ChannelImpl::Unsubscribe(std::string symbol) {
    symbol_controller_.GetInstrument(symbol).securityid;
  }

}}}
