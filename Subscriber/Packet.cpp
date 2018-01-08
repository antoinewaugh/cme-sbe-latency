#include "Packet.h"

bool Packet::HasNextMessage() {
  return processed_ < received_;
}

Message& Packet::NextMessage() {
   auto msgsize = message_.Reset(rawbytes_, processed_, received_); // message responsible for loading new values for msgsize, block_length, templateid ....
   processed_ += msgsize;
   return message_;
}

void Packet::Reset(char *data, size_t received) {
  rawbytes_ = data;
  received_ = received  ;
  processed_ = kByteOffest;
}

uint32_t Packet::GetSeqNum() {
  return seqnum_;
}

uint64_t Packet::GetSendTime() {
  return sendtime_;
}




