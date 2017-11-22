#include "Connection.h"

Connection::Connection(Type type, Feed feed, std::string host, short port)
    : type(type), feed(feed), host(boost::asio::ip::address::from_string(host)),
      port(port) {}