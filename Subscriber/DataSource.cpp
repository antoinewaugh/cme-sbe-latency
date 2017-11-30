#include "DataSource.h"

DataSource::DataSource(MulticastReceiver primary, MulticastReceiver secondary)
    : primary(std::move(primary)), secondary(std::move(secondary)) {}

void DataSource::Join() {
  primary.Join();
  secondary.Join();
}

void DataSource::Leave() {
  primary.Leave();
  secondary.Leave();
}

void DataSource::Register(std::function<void(char *, size_t)> callback) {
  primary.Register(callback);
  secondary.Register(callback);
}

std::ostream &operator<<(std::ostream &os, const DataSource &source) {
  os << "primary: " << source.primary << " secondary: " << source.secondary;
  return os;
}
