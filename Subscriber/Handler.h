#pragma once

#include "OrderBook.h"

class Handler {
public:
  // void OnQuote(OrderBook &book);
  void OnQuote(OrderBook &book, bool recovering, uint64_t securityid,
               uint64_t seqnum);
};
