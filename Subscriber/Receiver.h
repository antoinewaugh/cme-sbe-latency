#pragma once

#include "boost/bind.hpp"
#include <boost/asio.hpp>

#include "Decoder.h"

class Receiver {

  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint sender_endpoint_;
  boost::asio::ip::address listen_address_;
  boost::asio::ip::address multicast_address_;
  short multicast_port_;

  Decoder& decoder_;

  enum { max_length = 4096 };
  char data_[max_length];

  void HandleReceiveFrom(const boost::system::error_code &error,
                         size_t received);

public:
  Receiver(Decoder &decoder, boost::asio::io_service &io_service,
           const boost::asio::ip::address &listen_address,
           const boost::asio::ip::address &multicast_address,
           const short multicast_port);
  void Join();
  void Leave();
};