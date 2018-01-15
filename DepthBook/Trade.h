#pragma  once

#include <ostream>

class Trade {
  double price;
  int volume;
public:
  Trade(double price, int volume):price(price), volume(volume) {}

  friend std::ostream &operator<<(std::ostream &os, const Trade &trade) {
    os << "Trade >>  price: " << trade.price << " volume: " << trade.volume;
    return os;
  }
};

