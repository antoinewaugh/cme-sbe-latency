//
// Created by antoine on 27/11/17.
//

#include "DataSource.h"

DataSource::DataSource(boost::asio::io_service &io_service,
                       const boost::asio::ip::address &listen_address,
                       std::pair<Connection, Connection> conn,
                       std::function<void(char *, size_t)> callback)
    : primary(io_service, listen_address, conn.first, callback),
      secondary(io_service, listen_address, conn.first, callback) {}
