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

  // feed callbacks
  void OnIncrementalFeed(char *data, size_t bytes, NetworkPacketStatus);
  void OnSnapshotFeed(char *data, size_t bytes, NetworkPacketStatus);
  void OnInstrumentFeed(char *data, size_t bytes, NetworkPacketStatus);

  // decoder callbacks
  void OnChannelStatus(ChannelStatus status);

  // internal
  void HandleTimeout();
  void HandleMarketRecovered();
  void HandleInstrumentRecovered();

  void StartMarketRecovery();
  void StopMarketRecovery();
  void StartInstrumentRecovery();
  void StopInstrumentRecovery();

  // Client systems should begin recovering messages when MsgSeqNum = 1.
  // Tag 911-TotNumReports indicates the total number of messages to process for
  // a
  // complete set of Security Definitions. CME Group does not guarantee the
  // order of the messages being sent.
};
