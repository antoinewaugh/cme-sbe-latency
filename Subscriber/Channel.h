#pragma once

#include "MulticastReceiver.h"
#include "Connection.h"
#include "Packet.h"
#include "InstrumentMdHandler.h"
#include "ChannelController.h"

#include<string>

class Channel {

private:

  MulticastReceiver incrementala_;
  MulticastReceiver incrementalb_;
  MulticastReceiver snapshota_;
  MulticastReceiver snapshotb_;
  MulticastReceiver instrumenta_;
  MulticastReceiver instrumentb_;

  InstrumentMdHandler instruments_;
  ChannelController depthbooks_;
  void OnPacket(Type, Feed, Packet*);

public:
  Channel(std::string channelid,
          MulticastReceiver incrementala,
  MulticastReceiver incrementalb,
  MulticastReceiver snapshota,
  MulticastReceiver snapshotb,
  MulticastReceiver instrumenta,
  MulticastReceiver instrumentb);

  std::string GetId();
  std::string GetDescription();

  void Subscribe(std::string securityid);
  void Unsubscribe(std::string securityid);

  void StartIncrementalA();
  void StopIncrementalB();
  void StartSnapshotA();
  void StopSnapshotB();
  void StartInstrumentB();
  void StopInstrumentA();
  void StartIncrementalFeed();
  void StopIncrementalFeed();
  void StartSnapshotFeed();
  void StopSnapshotFeed();
  void StartInstrumentFeed();
  void StopInstrumentFeed();

  void StartAll();
  void StopAll();


};


