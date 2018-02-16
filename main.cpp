#include <iostream>

#include "boost/asio.hpp"
#include "config.h"
#include "ChannelImpl.h"
#include "MarketDataListener.h"

using namespace sp::lltp::cme;

int main(int argc, char *argv[]) {

  Handler handler;
  boost::asio::io_service io_service;
  auto listen_address = boost::asio::ip::address::from_string("0.0.0.0");
  auto configs = Config::load("config.xml");
  auto channel = Channel<Handler>::make_channel(handler, "310", configs, io_service, listen_address);
  channel->Subscribe("ESH8");
  io_service.run();
  return 0;
}

//int alt() {
//
//  sp::lltp::cme::Feed<Handler> feed("0.0.0.0","config.xml");
//  feeder.JoinChannel("310");
//  feeder.Subscribe("ES7", );
//  feeder.Subscribe("");
//  feeder.Subscribe("");
//  feeder.Subscribe("");
//}