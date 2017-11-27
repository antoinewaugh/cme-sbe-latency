#pragma once

#include "Connection.h"
#include "DataSource.h"
#include "Decoder.h"
#include "Handler.h"

class Subscriber {

  Config config_;
  Decoder decoder_;

  boost::asio::io_service &io_service_;
  const boost::asio::ip::address &listen_address_;

  DataSource incremental_feed_;
  std::unique_ptr<DataSource> snapshot_feed_;

  void OnData(char *data, size_t bytes) { decoder_.DecodePacket(data, bytes); }

  void OnSeqNumStatus(SeqNumStatus status) {

    if (status == Synchronized && snapshot_feed_) {
      snapshot_feed_.reset(nullptr); // leave recovery channel
    } else if (status == Unsynchronised && !snapshot_feed_) {
      snapshot_feed_ = std::unique_ptr<DataSource>(new DataSource(
          io_service_, listen_address_,
          config_.snapshot, // join recovery channel
          [this](char *data, size_t bytes) { this->OnData(data, bytes); }));
    }
  }

public:
  Subscriber(boost::asio::io_service &io_service,
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
};
