#include "Subscriber.h"

Subscriber::Subscriber(boost::asio::io_service &io_service,
             const boost::asio::ip::address &listen_address, uint64_t symbolid,
             Config config, Handler& handler)
      : config_(config),
        decoder_(symbolid, handler,
                 [this](auto &&val) { this->OnSeqNumStatus(val); }),
        io_service_(io_service), listen_address_(listen_address),
        incremental_feed_(
            io_service_, listen_address, config.incremental,
            [this](char *data, size_t bytes) { this->OnData(data, bytes); }) {

    snapshot_feed_ = std::unique_ptr<DataSource>(new DataSource(
        io_service_, listen_address_, config_.snapshot, // join recovery channel
        [this](char *data, size_t bytes) { this->OnData(data, bytes); }));
  }
