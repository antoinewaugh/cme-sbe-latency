#pragma once

#include "ChannelImpl.h"
#include "boost/asio.hpp"
#include <string>

namespace sp {
namespace lltp {
namespace cme {

class Feed {
public:
  void Feed(std::string const &multicast_ip, std::string const &config_xml);
  void JoinChannel(std::string const &channel);
  void Subscribe(std::string const &symbol);

private:
  Handler handler;
  Channel channel;
  boost::asio::io_service io_service;
  boost::asio::ip::address listen_address;
};

Feed::Feed(std::string const &multicast_ip, std::string const &config_xml)
    : listen_address(boost::asio::ip::address::from_string(multicast_ip)) {

  channel = Channel<Handler>::make_channel(handler, "925", configs, io_service, listen_address);
}

void Feed::Subscribe(std::string const &symbol) {
  channel->Subscribe(symbol);
}


//  channel->Subscribe("0GLBJ8");
//  channel->Subscribe("0GLBK8");
////  channel->Subscribe("3GLBM8"); // not in original list, ALSO
//  channel->Subscribe("0GLBJ8-0GLBK8");

  io_service.

  run();
};

}}}
