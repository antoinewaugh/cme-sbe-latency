#include "MulticastReceiver.h"

MulticastReceiver::MulticastReceiver(
    boost::asio::io_service &io_service,
    const boost::asio::ip::address &listen_address,
    const Connection &connection)
    : socket_(io_service), listen_address_(listen_address),
      connection_(connection) {}

MulticastReceiver::~MulticastReceiver() { Leave(); }

void MulticastReceiver::HandleReceiveFrom(
    const boost::system::error_code &error, size_t received) {

  if (!error) {

    // perform callback to user
    packet.Reset(data_, received);
    callback_(connection_.type, connection_.feed, &packet);

    // pull next message
    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        boost::bind(&MulticastReceiver::HandleReceiveFrom, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }
}

void MulticastReceiver::Leave() {
  if (joined_) {

    joined_ = false;
    socket_.close();
  }
}
void MulticastReceiver::Join() {

  if (joined_) {
    return;
  }

  joined_ = true;

  boost::asio::ip::udp::endpoint listen_endpoint(connection_.host.to_v4(),
                                                 connection_.port);

  socket_.open(listen_endpoint.protocol());
  socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
  socket_.set_option(boost::asio::ip::multicast::enable_loopback(true));

  socket_.bind(listen_endpoint);
  socket_.set_option(
      boost::asio::ip::multicast::join_group(connection_.host.to_v4()));

  socket_.async_receive_from(
      boost::asio::buffer(data_, max_length), sender_endpoint_,
      boost::bind(&MulticastReceiver::HandleReceiveFrom, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void MulticastReceiver::Register(std::function<void(Type, Feed, Packet*)> callback) {
  callback_ = callback;
}

std::ostream &operator<<(std::ostream &os, const MulticastReceiver &receiver) {
  os << "connection_: " << receiver.connection_;
  return os;
}

bool MulticastReceiver::IsActive() {
  return joined_;
}
