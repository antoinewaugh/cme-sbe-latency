#include "Packet.h"

#include <chrono>

// move to helper : exists in InstrumetnMdHandler.cpp too
static std::uint64_t NanosecondTimeDelta(uint64_t start, uint64_t stop) {
  return stop - start;
}

static std::uint64_t NanosecondTimeDeltaToNow(uint64_t start) {
  unsigned long ns_timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::system_clock::now().time_since_epoch())
      .count();
  return NanosecondTimeDelta(start, ns_timestamp);
}

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
  if(received >= processed_) {
    seqnum_ = SBE_LITTLE_ENDIAN_ENCODE_32(*((std::uint32_t *)(data)));
    sendtime_ = SBE_LITTLE_ENDIAN_ENCODE_64(*((std::uint64_t *)(data + sizeof(uint32_t)))); // offset = 4 butes for seqnum

    auto delay_ns = NanosecondTimeDeltaToNow(GetSendTime());
    auto delay_adj = delay_ns < 0? 0: delay_ns;
    std::cout << "Packet latency pre-decoding:" << delay_adj/1000 << " μs" <<  '\n';
  }
}

uint32_t Packet::GetSeqNum() {
  return seqnum_;
}

uint64_t Packet::GetSendTime() {
  return sendtime_;
}




