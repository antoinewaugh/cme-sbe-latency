#include <iostream>

#include "boost/asio.hpp"
#include "Config.h"
#include "Channel.h"
#include "Handler.h"



int main(int argc, char *argv[]) {

  Handler handler;

  boost::asio::io_service io_service;
  auto listen_address = boost::asio::ip::address::from_string("0.0.0.0");
  auto configs = Config::load("config.xml");
  auto channel = Channel<Handler>::make_channel(handler, "310", configs, io_service, listen_address);

  uint64_t ESH8 = 21168; // ESZ7;
  channel->Subscribe(ESH8);

  io_service.run();

  return 0;
}
