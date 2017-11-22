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
             Connection incremental_a, Connection incremental_b,
             Connection snapshot_a, Connection snapshot_b);
};
