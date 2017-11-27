#pragma once

#include "Connection.h"
#include "DataSource.h"
#include "Decoder.h"
#include "Handler.h"

struct Subscriber {

  Subscriber(boost::asio::io_service &io_service,
             const boost::asio::ip::address &listen_address, uint64_t symbolid,
             Config config, Handler& handler);

private:

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

};
