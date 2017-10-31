#include "Handler.h"
#include <iostream>

void Handler::OnQuote(OrderBook &book) { std::cout << book << '\n'; }