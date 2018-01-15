#pragma once

#include "DepthBook.h"
#include "Trade.h"

struct Handler {

  void OnQuote(DepthBook &book, uint64_t securityid,
               uint64_t seqnum);
  void OnTrade(Trade &trade, uint64_t securityid);
};
