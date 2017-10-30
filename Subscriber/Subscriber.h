#pragma once

#include "boost/bind.hpp"
#include <boost/asio.hpp>

#include "Decoder.h"

class Subscriber {

  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint sender_endpoint_;

  enum { max_length = 4096 };
  char data_[max_length];

  Decoder decoder;

  void handle_receive_from(const boost::system::error_code &error,
                           size_t received) {
    if (!error) {

      // decode
      decoder.decode_packet(data_, received);

      // pull next message
      socket_.async_receive_from(
          boost::asio::buffer(data_, max_length), sender_endpoint_,
          boost::bind(&Subscriber::handle_receive_from, this,
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
    }
  }

public:
  Subscriber(boost::asio::io_service &io_service,
           const boost::asio::ip::address &listen_address,
           const boost::asio::ip::address &multicast_address,
           const short multicast_port)
      : socket_(io_service) {

    boost::asio::ip::udp::endpoint listen_endpoint(boost::asio::ip::udp::v4(),
                                                   multicast_port);

    socket_.open(listen_endpoint.protocol());
    socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket_.set_option(boost::asio::ip::multicast::enable_loopback(true));

    socket_.bind(listen_endpoint);
    socket_.set_option(
        boost::asio::ip::multicast::join_group(multicast_address.to_v4()));

    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        boost::bind(&Subscriber::handle_receive_from, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }
};


