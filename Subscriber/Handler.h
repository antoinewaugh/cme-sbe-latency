#pragma once

#include "DepthBook.h"

struct Handler {
  void OnQuote(DepthBook &book, bool recovering, uint64_t securityid,
               uint64_t seqnum);
};
