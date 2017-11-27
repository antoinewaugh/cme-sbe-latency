#include "Connection.h"

Connection::Connection(std::string host, int port)
    : host(boost::asio::ip::address::from_string(host)), port(port) {}