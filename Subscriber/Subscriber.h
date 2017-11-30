#pragma once

#include "Config.h"
#include "DataSource.h"
#include "Decoder.h"
#include "Handler.h"

struct Subscriber {

  Subscriber(uint64_t symbolid, DataSource incremental_feed,
             DataSource snapshot_feed, DataSource instrument_feed,
             Handler &handler);

  Subscriber(Subscriber &&) = default;
  ~Subscriber();

  Subscriber static make_subscriber(uint64_t securityid, std::string channelid,
                                    Config &config,
                                    boost::asio::io_service &io_service,
                                    boost::asio::ip::address &listen_address,
                                    Handler &handler);

private:
  DataSource incremental_feed_;
  DataSource snapshot_feed_;
  DataSource instrument_feed_;

  Decoder decoder_;

  void OnData(char *data, size_t bytes);
  void OnSeqNumStatus(SeqNumStatus status);
};
