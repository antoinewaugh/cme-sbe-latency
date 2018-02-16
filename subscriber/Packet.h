#pragma once

#include <cstdint>
#include "Message.h"

class Packet {
public:

  void Reset(char* data, size_t received);
  uint32_t GetSeqNum();
  uint64_t GetSendTime();
  bool HasNextMessage();
  Message& NextMessage();

private:

  char* rawbytes_;
  uint32_t seqnum_ = 0;
  uint64_t sendtime_= 0;
  size_t processed_ = 0;
  size_t received_ = 0;
  Message message_;
};

