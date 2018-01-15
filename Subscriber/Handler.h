#pragma once

#include "DepthBook.h"
#include "Trade.h"
#include "SecurityStatus.h"
#include "SessionStatistics.h"

using sp::lltp::cme::SecurityStatus;
using sp::lltp::cme::SessionStatistics;

struct Handler {

  void OnQuote(uint64_t securityid, DepthBook const& book,
               uint64_t seqnum);
  void OnTrade(uint64_t securityid, Trade &trade );
  void OnStatus(uint64_t securityid, SecurityStatus status);
  void OnStatistics(uint64_t securityid, SessionStatistics&);
};
