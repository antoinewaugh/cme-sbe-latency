#include <iostream>

#include "boost/asio.hpp"
#include "Config.h"
#include "Channel.h"
#include "Handler.h"


int main(int argc, char *argv[]) {

//  std::unordered_map<std::uint16_t, Channel> channels;
//  auto channels = {"310", "311"};
//
//  ChannelService service = make_service(config, channels, handler);
//  service.subscribe();
//  service.subscribe();
//  service.start();

  Handler handler;

//
//  for(auto&& channel: channels ) {
//    std::cout << channel << '\n';
//  }


  boost::asio::io_service io_service;
 // boost::asio::io_service::work work(io_service);

//  auto listen_address = boost::asio::ip::address::from_string("0.0.0.0");
  auto listen_address = boost::asio::ip::address::from_string("10.144.138.2");
  //auto configs = Config::load("config.xml");
//  auto channel = Channel<Handler>::make_channel(handler, "310", configs, io_service, listen_address);
  auto configs = Config::load("config-conformance.xml");
  auto channel = Channel<Handler>::make_channel(handler, "925", configs, io_service, listen_address);

  //channel->Subscribe(21168);
  channel->Subscribe("0GLBJ8");
//  channel->Subscribe("ESH8");



  io_service.run();

  return 0;
}
