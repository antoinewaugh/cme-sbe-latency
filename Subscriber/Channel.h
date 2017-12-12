#pragma once

#include "MulticastReceiver.h"
#include "Connection.h"
#include "Packet.h"
#include "InstrumentMdHandler.h"
#include "ChannelAccessor.h"
#include "ChannelController.h"

#include<string>
#include<memory>
#include "Config.h"

class Channel: public ChannelAccessor {

private:

  std::string channelid_;
  MulticastReceiver incrementala_;
  MulticastReceiver incrementalb_;
  MulticastReceiver snapshota_;
  MulticastReceiver snapshotb_;
  MulticastReceiver instrumenta_;
  MulticastReceiver instrumentb_;

//  InstrumentMdHandler instruments_;
  ChannelController channel_controller_;
  bool IsSnapshotFeedActive();
  void OnPacket(Type, Feed, Packet*);

public:
  Channel(std::string channelid,
  MulticastReceiver incrementala,
  MulticastReceiver incrementalb,
  MulticastReceiver snapshota,
  MulticastReceiver snapshotb,
  MulticastReceiver instrumenta,
  MulticastReceiver instrumentb);

  Channel(Channel &&) = delete;
  std::string GetId();

  void Subscribe(uint32_t securityid);
  void Unsubscribe(uint32_t
                   securityid);

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

  void SubscribeToSnapshotsForInstrument(uint32_t securityid);
  void UnsubscribeToSnapshotsForInstrument(uint32_t securityid);

  void StartAll();
  void StopAll();

  static std::unique_ptr<Channel> make_channel(
                                   std::string channelid, Config &config,
                                   boost::asio::io_service &io_service,
                                   boost::asio::ip::address &listen_address
  );
};


