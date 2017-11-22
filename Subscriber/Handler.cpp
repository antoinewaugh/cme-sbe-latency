#include "Handler.h"
#include <iostream>

// Assumes ANSI compatible Terminal
// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
// https://rosettacode.org/wiki/Terminal_control/Clear_the_screen

static void clear() { std::cout << "\x1B[2J\x1B[H"; }

void Handler::OnQuote(DepthBook &book, bool recovering, uint64_t securityid,
                      uint64_t seqnum) {
  clear();

  std::cout << "Security: " << securityid << ", Recovering: " << recovering
            << ", LastSeqNum: " << seqnum << '\n';

  std::cout << book << '\n';
}