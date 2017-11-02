#pragma once

#include "Decoder.h"
#include "Handler.h"
#include "SymbolFeed.h"

class Subscriber {

  Handler handler_;
  Decoder decoder_;
  SymbolFeed feed_;

public:
  Subscriber(boost::asio::io_service &io_service,
                      const boost::asio::ip::address &listen_address,
                      const boost::asio::ip::address &multicast_address,
                      const short multicast_port);
};
