#include <iostream>

#include "boost/asio.hpp"
#include "Config.h"
#include "Channel.h"
#include "Handler.h"

using namespace sp::lltp::cme;

int main(int argc, char *argv[]) {


  Handler handler;
  boost::asio::io_service io_service;
//  auto listen_address = boost::asio::ip::address::from_string("0.0.0.0");
  auto listen_address = boost::asio::ip::address::from_string("10.144.138.2");
  //auto configs = Config::load("config.xml");
//  auto channel = Channel<Handler>::make_channel(handler, "310", configs, io_service, listen_address);
  auto configs = Config::load("config-conformance.xml");
  auto channel = Channel<Handler>::make_channel(handler, "925", configs, io_service, listen_address);
//  auto channel = Channel<Handler>::make_channel(handler, "924", configs, io_service, listen_address);

  channel->Subscribe("0GLBU8");
  channel->Subscribe("0GLBV8");
  channel->Subscribe("0GLBU8-0GLBV8");

//  channel->Subscribe("0GLBJ8");
//  channel->Subscribe("0GLBK8");
////  channel->Subscribe("3GLBM8"); // not in original list, ALSO
//  channel->Subscribe("0GLBJ8-0GLBK8");

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