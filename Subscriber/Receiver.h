#pragma once

#include "boost/bind.hpp"
#include <boost/asio.hpp>

#include "Connection.h"
#include "Decoder.h"

class Receiver {

  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint sender_endpoint_;
  boost::asio::ip::address listen_address_;
  Connection connection_;

  Decoder &decoder_;

  enum { max_length = 4096 };
  char data_[max_length];

  void HandleReceiveFrom(const boost::system::error_code &error,
                         size_t received);

public:
  Receiver(Decoder &decoder, boost::asio::io_service &io_service,
           const boost::asio::ip::address &listen_address,
           Connection connection);
  void Join();
  void Leave();
};