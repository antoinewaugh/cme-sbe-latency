#pragma once

#include <functional>

#include "boost/bind.hpp"
#include <boost/asio.hpp>
#include <ostream>

#include "Connection.h"
#include "Decoder.h"

struct MulticastReceiver {

  MulticastReceiver(boost::asio::io_service &io_service,
                    const boost::asio::ip::address &listen_address,
                    const Connection &connection);
  ~MulticastReceiver();
  MulticastReceiver(MulticastReceiver&& ) = default;
  void Register(std::function<void(char *, size_t)> callback);
  void Join();
  void Leave();

private:
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint sender_endpoint_;
  boost::asio::ip::address listen_address_;
  Connection connection_;

  std::function<void(char *, size_t)> callback_;

  static constexpr int max_length = 4096;
  char data_[max_length];

  bool joined_ = false;

  void HandleReceiveFrom(const boost::system::error_code &error,
                         size_t received);

public:
  friend std::ostream &operator<<(std::ostream &os, const MulticastReceiver &receiver);
};