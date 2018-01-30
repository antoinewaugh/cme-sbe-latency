#pragma once

#include "DepthBook.h"
#include "Trade.h"
#include "SecurityStatus.h"
#include "SessionStatistics.h"

using sp::lltp::cme::SecurityStatus;
using sp::lltp::cme::SecurityEvent;
using sp::lltp::cme::SessionStatistics;

struct Handler {

  virtual void OnQuote(uint64_t securityid, DepthBook const& book) = 0;
  virtual void OnTrade(uint64_t securityid, Trade &trade ) = 0;
  virtual void OnStatus(uint64_t securityid, SecurityStatus status, SecurityEvent event) = 0;
  virtual void OnStatistics(uint64_t securityid, SessionStatistics&) = 0;
};
