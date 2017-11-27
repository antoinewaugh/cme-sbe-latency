#pragma once

#include "Connection.h"
#include "Receiver.h"

#include <functional>

struct DataSource {

  Receiver primary, secondary;

  DataSource(boost::asio::io_service &io_service,
             const boost::asio::ip::address &listen_address,
             std::pair<Connection, Connection> conn,
             std::function<void(char *, size_t)> callback);
};
