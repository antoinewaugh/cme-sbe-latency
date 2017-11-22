#pragma once

#include <string>

#include <boost/asio.hpp>

struct Connection {

  enum Type { Incremental, Snapshot };
  enum Feed { Primary, Secondary };

  Type type;
  Feed feed;

  boost::asio::ip::address host;
  short port;

  Connection(Type type, Feed feed, std::string host, short port);
};
