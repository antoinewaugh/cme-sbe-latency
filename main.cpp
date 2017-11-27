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
            << argv[4] << " " << argv[5] << " " << argv[6]
            << argv[7] << " " << argv[8] << " " << argv[9]
            << '\n';

  boost::asio::io_service io_service;

  uint64_t ESZ7 = 23936; // ESZ7;

  Handler handler;
  Config config = Config(
      std::make_pair(Connection(argv[2], atoi(argv[3])),Connection(argv[2], atoi(argv[3]))),
      std::make_pair(Connection(argv[2], atoi(argv[3])),Connection(argv[2], atoi(argv[3])))
  );

  Subscriber(io_service,boost::asio::ip::address::from_string(argv[1]), ESZ7, config, handler);

  io_service.run();

  return 0;
}
