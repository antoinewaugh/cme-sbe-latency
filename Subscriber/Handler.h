#pragma once

#include "DepthBook.h"

class Handler {
public:
  void OnQuote(DepthBook &book, bool recovering, uint64_t securityid,
               uint64_t seqnum);
};
