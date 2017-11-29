#pragma once

#include <string>

#include <boost/asio.hpp>

enum Type { Snapshot, Incremental, Instrument, Other };
enum Feed { A, B };

#include <map>

static Type get_type(std::string const &type) {
  static std::map<std::string, Type> const conversion = {
      {"Instrument", Type::Instrument},
      {"Incremental", Type::Incremental},
      {"Snapshot", Type::Snapshot},
  };

  auto const search = conversion.find(type);
  if (search == conversion.end())
    return Other;

  return search->second;
}

static Feed get_feed(std::string const &feed) {
  return (feed == "A") ? Feed::A : Feed::B;
}
struct Connection {

  std::string id;
  boost::asio::ip::address host;
  int port;

  Type type;
  Feed feed;

  Connection() = default;
  Connection(std::string id, Type type, Feed feed, std::string host, int port);
};
