#pragma once

#include "DepthBook.h"
#include "Trade.h"
#include "SecurityStatus.h"

using sp::lltp::cme::SecurityStatus;

struct Handler {

  void OnQuote(uint64_t securityid, DepthBook const& book,
               uint64_t seqnum);
  void OnTrade(uint64_t securityid, Trade &trade );
  void OnStatus(uint64_t securityid, SecurityStatus status);
};
