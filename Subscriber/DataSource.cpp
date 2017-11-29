#include "DataSource.h"

DataSource::DataSource(std::unique_ptr<MulticastReceiver> primary,
                       std::unique_ptr<MulticastReceiver> secondary)
    : primary(std::move(primary)), secondary(std::move(secondary)) {}

void DataSource::Join() {
  primary->Join();
  secondary->Join();
}

void DataSource::Leave() {
  primary->Leave();
  secondary->Leave();
}
