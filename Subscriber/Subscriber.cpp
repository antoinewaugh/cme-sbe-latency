#include "Subscriber.h"
Subscriber::Subscriber(uint64_t symbolid, DataSource incremental_feed,
                       DataSource snapshot_feed, DataSource instrument_feed,
                       Handler &handler)

    : incremental_feed_(std::move(incremental_feed)),
      snapshot_feed_(std::move(snapshot_feed)),
      instrument_feed_(std::move(instrument_feed)),
      decoder_(symbolid, handler,
               [this](auto &&val) { this->OnChannelStatus(val); }) {

  incremental_feed_.RegisterCallback([this](char *d, size_t r, NetworkPacketStatus s) {
    this->OnIncrementalFeed(d, r, s);
  });

  snapshot_feed_.RegisterCallback([this](char *d, size_t r, NetworkPacketStatus s) {
    this->OnSnapshotFeed(d, r, s);
  });

  instrument_feed_.RegisterCallback([this](char *d, size_t r, NetworkPacketStatus s) {
    this->OnInstrumentFeed(d, r, s);
  });

  StartMarketRecovery();
  StartInstrumentRecovery();

}

Subscriber::~Subscriber() {
  incremental_feed_.Leave();
  snapshot_feed_.Leave();
  instrument_feed_.Leave();
}

Subscriber Subscriber::make_subscriber(uint64_t securityid,
                                       std::string channelid, Config &config,
                                       boost::asio::io_service &io_service,
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

void Subscriber::StartMarketRecovery() {
  decoder_.Clear(); // enters recovery mode
  incremental_feed_.Join();
  snapshot_feed_.Join();
}

void Subscriber::StartInstrumentRecovery() {
  instrument_feed_.Join();
}


// Feed callbacks

void Subscriber::OnIncrementalFeed(char *data, size_t bytes, NetworkPacketStatus status)
{
  if (status == Expected) {
    decoder_.DecodePacket(data, bytes);
  } else if (status == RecoveryRequired) {
    StartMarketRecovery();
    StartInstrumentRecovery();
  }
}

// Gap detection not necessary as we are only subscribed in recovery mode
void Subscriber::OnSnapshotFeed(char *data, size_t bytes,
                                NetworkPacketStatus status) {
  if(!status == Redundant) {
    decoder_.DecodePacket(data, bytes);
  }
}

// Gap detection not necessary as we are only subscribed in recovery mode
void Subscriber::OnInstrumentFeed(char *data, size_t bytes,
                                  NetworkPacketStatus status) {
  if(!status == Redundant) {
    decoder_.DecodePacket(data, bytes);
  }
}

// Decoder callbacks : status known after decoding message(s)

void Subscriber::OnChannelStatus(ChannelStatus status) {
  switch (status) {
  case TimeoutIdentified:
    HandleTimeout();
    break;
  case MarketRecovered:
    StopMarketRecovery();
    break;
  case InstrumentsRecovered:
    StopInstrumentRecovery();
    break;
  }
}

void Subscriber::HandleTimeout() {
  // LOG!!!
  decoder_.Clear();
  incremental_feed_.Reset();
  snapshot_feed_.Reset();
  instrument_feed_.Reset();
}

void Subscriber::StopMarketRecovery() { snapshot_feed_.Leave(); }

void Subscriber::StopInstrumentRecovery() { instrument_feed_.Leave(); }

// Client systems should begin recovering messages when MsgSeqNum = 1.
// Tag 911-TotNumReports indicates the total number of messages to process for a
// complete set of Security Definitions. CME Group does not guarantee the
// order of the messages being sent.
