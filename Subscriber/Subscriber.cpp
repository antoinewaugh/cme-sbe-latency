#include "Subscriber.h"

Subscriber::Subscriber(uint64_t symbolid,
                       DataSource incremental_feed,
                       DataSource snapshot_feed,
                       DataSource instrument_feed,
                       Handler &handler)

    : incremental_feed_(std::move(incremental_feed)),
      snapshot_feed_(std::move(snapshot_feed)),
      instrument_feed_(std::move(instrument_feed)),
      decoder_(symbolid, handler,
               [this](auto &&val) { this->OnSeqNumStatus(val); }) {


  // wish i could remove this - dependency injection has caused it? at least move it to be available at the DataSource level..
  incremental_feed.primary.Register([this](char* d, size_t r) { this->OnData(d,r); });
  incremental_feed.secondary.Register([this](char* d, size_t r) { this->OnData(d,r); });
  snapshot_feed_.primary.Register([this](char* d, size_t r) { this->OnData(d,r); });
  snapshot_feed_.secondary.Register([this](char* d, size_t r) { this->OnData(d,r); });
  instrument_feed_.primary.Register([this](char* d, size_t r) { this->OnData(d,r); });
  instrument_feed_.secondary.Register([this](char* d, size_t r) { this->OnData(d,r); });


  incremental_feed_.Join();
  snapshot_feed_.Join();
  instrument_feed_.Join();
}

void Subscriber::OnData(char *data, size_t bytes) {
  decoder_.DecodePacket(data, bytes);
}

void Subscriber::OnSeqNumStatus(SeqNumStatus status) {

  if (status == Synchronized) {
    snapshot_feed_.Leave(); // leave recovery channel
  } else if (status == Unsynchronised) {
    snapshot_feed_.Join();   // join recovery
    instrument_feed_.Join(); // join recovery
  }
}

Subscriber::~Subscriber() {
  incremental_feed_.Leave();
  snapshot_feed_.Leave();
  instrument_feed_.Leave();
}

Subscriber
Subscriber::make_subscriber(uint64_t securityid, std::string channelid,
                            Config &config, boost::asio::io_service &io_service,
                            boost::asio::ip::address &listen_address,
                            Handler &handler) {

  auto incremental = DataSource(
      std::move(MulticastReceiver(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Incremental, Feed::A))),
      std::move(MulticastReceiver(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Incremental, Feed::B))));

  auto snapshot = DataSource(
      std::move(MulticastReceiver(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Snapshot, Feed::A))),
      std::move(MulticastReceiver(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Snapshot, Feed::B))));

  auto instrument = DataSource(
      std::move(MulticastReceiver(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Instrument, Feed::A))),
      std::move(MulticastReceiver(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Instrument, Feed::B))));

  return Subscriber(securityid, std::move(incremental), std::move(snapshot),
                                      std::move(instrument), handler);
}
