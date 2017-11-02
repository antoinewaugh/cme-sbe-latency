#pragma once

#include "OrderBook.h"

class Handler {
public:
  void OnQuote(OrderBook &book);
};
