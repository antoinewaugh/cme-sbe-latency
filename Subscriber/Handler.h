#pragma once

#include "OrderBook.h"

class Handler {
  //  SymbolFeed& feed_;
public:
  //  Handler(SymbolFeed);
  void OnQuote(OrderBook &book);
};
