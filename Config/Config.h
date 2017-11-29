#pragma once

#include "Connection.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <array>
#include <ostream>
#include <utility>

struct ChannelConfig {

  std::string id;
  std::string label;

  void AddProduct(std::string product);

  void AddConnection(Connection);

  std::vector<std::string> products;
  std::array<Connection, 2> incremental;
  std::array<Connection, 2> snapshot;
  std::array<Connection, 2> instrument;
};

struct Config {
  Connection GetConnection(std::string channelid, Type, Feed);
  void AddChannel(ChannelConfig channel);
  static Config load(std::string filename);

private:
  std::map<std::string, ChannelConfig> configs_;
};
