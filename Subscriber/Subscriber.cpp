#include "Subscriber.h"

Subscriber::Subscriber(uint64_t symbolid,
                       std::unique_ptr<DataSource> incremental_feed,
                       std::unique_ptr<DataSource> snapshot_feed,
                       std::unique_ptr<DataSource> instrument_feed,
                       Handler &handler)

    : incremental_feed_(std::move(incremental_feed)),
      snapshot_feed_(std::move(snapshot_feed)),
      instrument_feed_(std::move(instrument_feed)),
      decoder_(symbolid, handler,
               [this](auto &&val) { this->OnSeqNumStatus(val); }) {

  incremental_feed_->Join();
  snapshot_feed_->Join();
  instrument_feed_->Join();
}

void Subscriber::OnData(char *data, size_t bytes) {
  decoder_.DecodePacket(data, bytes);
}

void Subscriber::OnSeqNumStatus(SeqNumStatus status) {

  if (status == Synchronized) {
    snapshot_feed_->Leave(); // leave recovery channel
  } else if (status == Unsynchronised) {
    snapshot_feed_->Join();   // join recovery
    instrument_feed_->Join(); // join recovery
  }
}

Subscriber::~Subscriber() {
  incremental_feed_->Leave();
  snapshot_feed_->Leave();
  instrument_feed_->Leave();
}

std::unique_ptr<Subscriber>
Subscriber::make_subscriber(uint64_t securityid, std::string channelid,
                            Config &config, boost::asio::io_service &io_service,
                            boost::asio::ip::address &listen_address,
                            Handler &handler) {

  auto incremental = std::make_unique<DataSource>(
      std::make_unique<MulticastReceiver>(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Incremental, Feed::A)),
      std::make_unique<MulticastReceiver>(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Incremental, Feed::B)));

  auto snapshot = std::make_unique<DataSource>(
      std::make_unique<MulticastReceiver>(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Snapshot, Feed::A)),
      std::make_unique<MulticastReceiver>(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Snapshot, Feed::B)));

  auto instrument = std::make_unique<DataSource>(
      std::make_unique<MulticastReceiver>(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Instrument, Feed::A)),
      std::make_unique<MulticastReceiver>(
          io_service, listen_address,
          config.GetConnection(channelid, Type::Instrument, Feed::B)));

  return std::make_unique<Subscriber>(securityid, incremental, snapshot,
                                      instrument, handler);
}
