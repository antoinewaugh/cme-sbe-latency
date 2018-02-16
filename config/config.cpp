#include "config.h"

void ChannelConfig::AddProduct(std::string product) {
  products.push_back(product);
}

void ChannelConfig::AddConnection(Connection connection) {

  if (connection.type == Incremental) {
    incremental[connection.feed] = connection;
  } else if (connection.type == Snapshot) {
    snapshot[connection.feed] = connection;
  } else if (connection.type == Instrument) {
    instrument[connection.feed] = connection;
  }
}

Config Config::load(std::string filename) {

  Config configs;

  using boost::property_tree::ptree;
  ptree pt;

  read_xml(filename, pt);

  for (auto &&channel : pt.get_child("configuration")) {
    if (channel.first == "channel") {

      auto id = channel.second.get<std::string>("<xmlattr>.id", "");
      auto label = channel.second.get<std::string>("<xmlattr>.label", "");

      ChannelConfig config;
      config.id = id;
      config.label = label;

      for (auto &&product : channel.second.get_child("products")) {
        if (product.first == "product") {
          config.AddProduct(
              product.second.get<std::string>("<xmlattr>.code", ""));
        }
      }

      for (auto &&connection : channel.second.get_child("connections")) {
        if (connection.first == "connection") {

          auto id = connection.second.get<std::string>("<xmlattr>.ip", "");
          auto host = connection.second.get<std::string>("ip", "");
          auto port = connection.second.get<int>("port", 0);
          auto type = get_type(connection.second.get<std::string>("type", ""));
          auto feed = get_feed(connection.second.get<std::string>("feed", ""));

          // Only Add Multicast Details
          if (type != Other) {
            Connection connection(id, type, feed, host, port);
            config.AddConnection(connection);
          }
        }
      }
      configs.AddChannel(config);
    }
  }

  return configs;
}

Connection Config::GetConnection(std::string channelid, Type type, Feed feed) {
  if (type == Incremental) {
    return configs_[channelid].incremental[feed];
  } else if (type == Snapshot) {
    return configs_[channelid].snapshot[feed];
  } else if (type == Instrument) {
    return configs_[channelid].instrument[feed];
  }
}

void Config::AddChannel(ChannelConfig channel) {
  configs_.emplace(channel.id, channel);
}
