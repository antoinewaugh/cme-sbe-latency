#include "Packet.h"

#include <chrono>

bool Packet::HasNextMessage() {
  return processed_ < received_;
}

Message& Packet::NextMessage() {
   auto msgsize = message_.Reset(rawbytes_, processed_, received_); // message responsible for loading new values for msgsize, block_length, templateid ....
   processed_ += msgsize;
   return message_;
}

void Packet::Reset(char *data, size_t received) {
    rec_ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
  rawbytes_ = data;
  received_ = received  ;
  processed_ = kByteOffest;
  if(received > kByteOffest) {
    seqnum_ = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *) (data)));
    sendtime_ = SBE_LITTLE_ENDIAN_ENCODE_64(*((std::uint64_t *) (data + sizeof(uint32_t))));
  }
}

uint32_t Packet::GetSeqNum() {
  return seqnum_;
}

uint64_t Packet::GetSendTime() {
  return sendtime_;
}




