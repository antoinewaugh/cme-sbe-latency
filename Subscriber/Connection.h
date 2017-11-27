#pragma once

#include <string>

#include <boost/asio.hpp>

struct Connection {
  boost::asio::ip::address host;
  int port;
  Connection(std::string, int);
};

struct Config {
  std::pair<Connection, Connection> incremental;
  std::pair<Connection, Connection> snapshot;
  Config(std::pair<Connection, Connection> incremental,
         std::pair<Connection, Connection> snapshot)
      : incremental(incremental), snapshot(snapshot) {}
};
