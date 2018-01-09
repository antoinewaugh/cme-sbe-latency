#include <iostream>

#include "boost/asio.hpp"
#include "Config.h"
#include "Channel.h"

int main(int argc, char *argv[]) {

  uint64_t ESH8 = 21168;
  boost::asio::io_service io_service;
  auto listen_address = boost::asio::ip::address::from_string("0.0.0.0");
  auto configs = Config::load("config.xml");
  auto channel = Channel::make_channel("310", configs, io_service, listen_address);

  channel->Subscribe(ESH8);

  io_service.run();

  return 0;
}
