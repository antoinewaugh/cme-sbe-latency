#pragma once

#include "Config.h"
#include "DataSource.h"
#include "Decoder.h"
#include "Handler.h"

struct Subscriber {

  Subscriber(uint64_t symbolid, std::unique_ptr<DataSource> incremental_feed,
             std::unique_ptr<DataSource> snapshot_feed,
             std::unique_ptr<DataSource> instrument_feed, Handler &handler);

  ~Subscriber();

  std::unique_ptr<Subscriber>
  make_subscriber(uint64_t securityid, std::string channelid, Config &config,
                  boost::asio::io_service &io_service,
                  boost::asio::ip::address &listen_address, Handler &handler);

private:
  std::unique_ptr<DataSource> incremental_feed_;
  std::unique_ptr<DataSource> snapshot_feed_;
  std::unique_ptr<DataSource> instrument_feed_;

  Decoder decoder_;

  void OnData(char *data, size_t bytes);
  void OnSeqNumStatus(SeqNumStatus status);
};
