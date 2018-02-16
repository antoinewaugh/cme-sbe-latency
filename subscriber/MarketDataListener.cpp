#include "MarketDataListener.h"
#include <iostream>

// Assumes ANSI compatible Terminal
// CSI[2J clears screen, CSI[H moves the cursor to top-left corner
// https://rosettacode.org/wiki/Terminal_control/Clear_the_screen

static void clear() { std::cout << "\x1B[2J\x1B[H"; }

void MarketDataListener::OnQuote(uint64_t securityid,DepthBook const& book) {
// clear();

  std::cout << "*********** Quote ***************\n";
  std::cout << securityid << "\n";
  std::cout << "*********************************\n";
  std::cout << book << '\n';
}

void MarketDataListener::OnTrade(uint64_t securityid, Trade& trade) {
  std::cout << "*********** Trade ***************\n";
  std::cout << securityid << "\n";
  std::cout << "*********************************\n";
  std::cout << trade << '\n';
}

void MarketDataListener::OnStatus(uint64_t securityid, SecurityStatus status, SecurityEvent event) {
  std::cout << "*********** Status ***************\n";
  std::cout << securityid << "\n";
  std::cout << "*********************************\n";
  std::cout << "Status: >> " << (int)status << " Event:" << (int)event << '\n';
}

void MarketDataListener::OnStatistics(uint64_t securityid, SessionStatistics &statistics) {
  std::cout << "*********** Statistics ***************\n";
  std::cout << securityid << "\n";
  std::cout << "*********************************\n";
  std::cout << statistics << '\n';
}
