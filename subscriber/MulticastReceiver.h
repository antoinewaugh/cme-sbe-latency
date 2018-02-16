#pragma once

#include "Packet.h"
#include "Connection.h"

#include <functional>

#include "boost/bind.hpp"
#include <boost/asio.hpp>
#include <ostream>


struct MulticastReceiver {

  MulticastReceiver(boost::asio::io_service &io_service,
                    const boost::asio::ip::address &listen_address,
                    const Connection &connection);
  ~MulticastReceiver();
  MulticastReceiver(MulticastReceiver &&) = default;
  void Register(std::function<void(Type, Feed, Packet*)> callback);
  void Join();
  void Leave();
  bool IsActive();

private:
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint sender_endpoint_;
  boost::asio::ip::address listen_address_;
  Connection connection_;

  std::function<void(Type, Feed, Packet*)> callback_;

  static constexpr int max_length = 4096;
  char data_[max_length];

  bool joined_ = false;

  void HandleReceiveFrom(const boost::system::error_code &error,
                         size_t received);

  Packet packet;

public:
  friend std::ostream &operator<<(std::ostream &os,
                                  const MulticastReceiver &receiver);
};