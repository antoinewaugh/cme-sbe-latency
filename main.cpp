#include "Subscriber.h"
#include <iostream>

int main(int argc, char *argv[]) {

  if (argc != 10) {
    std::cerr << "Usage: receiver <listen_address> <multicast_address> "
                 "<multicast_port>\n";
    std::cerr << "  For IPv4, try:\n";
    std::cerr << "    receiver 0.0.0.0 224.0.31.1 14310 224.0.32.1 15310 224.0.31.22 14310 224.0.32.22 15310 \n";
    return 1;
  }

  std::cout << "Starting : " << argv[1] << " " << argv[2] << " " << argv[3]
            << '\n';

  boost::asio::io_service io_service;

  Subscriber r(io_service, boost::asio::ip::address::from_string(argv[1]),
               Connection(Connection::FeedType::Incremental, Connection::Feed::A, argv[2], atoi(argv[3])),
               Connection(Connection::FeedType::Incremental, Connection::Feed::B, argv[4], atoi(argv[5])),
               Connection(Connection::FeedType::Snapshot, Connection::Feed::A, argv[6], atoi(argv[7])),
               Connection(Connection::FeedType::Snapshot, Connection::Feed::B, argv[8], atoi(argv[9]))
  );

  io_service.run();

  return 0;
}
