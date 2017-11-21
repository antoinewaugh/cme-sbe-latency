#include "Handler.h"
#include <iostream>

void clear() {
  // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
  std::cout << "\x1B[2J\x1B[H";
}

void Handler::OnQuote(OrderBook &book, bool recovering, uint64_t securityid, uint64_t seqnum) {
  clear();

  std::cout << securityid << " Recovering " << recovering << ", " << seqnum << '\n';
  std::cout << book << '\n';
}