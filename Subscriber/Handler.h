#pragma once

#include "OrderBook.h"
#include "SymbolFeed.h"

class Handler {
//  SymbolFeed& feed_;
public:
//  Handler(SymbolFeed);
  void OnQuote(OrderBook& book);
};

