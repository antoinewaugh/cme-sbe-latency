#include "Subscriber.h"

Subscriber::Subscriber(boost::asio::io_service &io_service,
                       const boost::asio::ip::address &listen_address,
                       Connection incremental_a, Connection incremental_b,
                       Connection snapshot_a, Connection snapshot_b)
    : feed_(23936, handler_, decoder_, io_service, listen_address,
            incremental_a, incremental_b, snapshot_a, snapshot_b) {}
