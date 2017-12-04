#include "DataSource.h"

DataSource::DataSource(MulticastReceiver primary, MulticastReceiver secondary)
    : primary(std::move(primary)), secondary(std::move(secondary)) {


}

void DataSource::RegisterCallback(
    std::function<void(char *, size_t, NetworkPacketStatus)> callback) {

  callback_ = callback;

  primary.Register([this](char* d, size_t r){this->OnData(d,r);});
  secondary.Register([this](char* d, size_t r){this->OnData(d,r);});

}

void DataSource::Join() {
  primary.Join();
  secondary.Join();
}

void DataSource::Leave() {
  primary.Leave();
  secondary.Leave();
}

void DataSource::Reset() {
  Leave();
  Join();
}

NetworkPacketStatus DataSource::GetNetworkPacketStatus(uint32_t seqnum) {

  if (seqnum <= lastseqnum_) { return Redundant; }

  auto expected_seqnum = std::exchange(lastseqnum_, seqnum) + 1;
  if (expected_seqnum == seqnum) { return Expected; };
  if (expected_seqnum > seqnum)  { return RecoveryRequired; };

}

// this needs to be implmented to view primary and secondary
void DataSource::OnDataPrimary(char *data, size_t received) {

}
// this needs to be implmented to view primary and secondary
void DataSource::OnDataSeconary(char *data, size_t received) {

}
void DataSource::OnData(char *data, size_t received) {

  auto seqnum = Decoder::GetSeqNum(data,received);
  auto status = GetNetworkPacketStatus(seqnum);
  if(!status == Redundant) {
    callback_(data, received, status);
  }
}

std::ostream &operator<<(std::ostream &os, const DataSource &source) {
  os << "primary: " << source.primary << " secondary: " << source.secondary;
  return os;
}

