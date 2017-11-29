#include "DataSource.h"

DataSource::DataSource(MulticastReceiver primary,
                       MulticastReceiver secondary)
    : primary(std::move(primary)), secondary(std::move(secondary)) {}

void DataSource::Join() {
  primary.Join();
  secondary.Join();
}

void DataSource::Leave() {
  primary.Leave();
  secondary.Leave();
}
