#include "Handler.h"
#include <iostream>

// Assumes ANSI compatible Terminal
// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
// https://rosettacode.org/wiki/Terminal_control/Clear_the_screen

static void clear() { std::cout << "\x1B[2J\x1B[H"; }

void Handler::OnQuote(uint64_t securityid,DepthBook const& book,
                      uint64_t seqnum) {
  clear();
  std::cout << book << '\n';
}

void Handler::OnTrade(uint64_t securityid, Trade& trade) {
  std::cout << trade<< '\n';
}

void Handler::OnStatus(uint64_t securityid, SecurityStatus status) {
  std::cout << "Status >> " << (int)status << '\n';
}
