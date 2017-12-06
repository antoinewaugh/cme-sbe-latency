#pragma once

#include <cstdint>
#include "Message.h"

class Packet {
public:
  uint32_t GetSeqNum();
  bool HasNext();
  Message* Next();

private:
  Message current_message_;
};

