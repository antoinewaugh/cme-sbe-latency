#include "Subscriber.h"

Subscriber::Subscriber(boost::asio::io_service &io_service,
                       const boost::asio::ip::address &listen_address,
                       const boost::asio::ip::address &multicast_address,
                       const short multicast_port)
    : feed_(23936, handler_, decoder_, io_service, listen_address,
            multicast_address, multicast_port) {}
