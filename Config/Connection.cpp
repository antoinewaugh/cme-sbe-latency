#include "Connection.h"

Connection::Connection(std::string id, Type type, Feed feed, std::string host,
                       int port)
    : id(id), type(type), feed(feed),
      host(boost::asio::ip::address::from_string(host)), port(port) {}

std::ostream &operator<<(std::ostream &os, const Connection &connection) {
  os << "id: " << connection.id << " host: " << connection.host << " port: " << connection.port << " type: "
     << connection.type << " feed: " << connection.feed;
  return os;
}
