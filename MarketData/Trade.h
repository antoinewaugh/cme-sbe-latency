#pragma  once

#include <ostream>

class Trade {
public:
  Trade(std::string symbol, double price, int volume, int updateaction):symbol(symbol), price(price), volume(volume),updateaction(updateaction)  {}
  Trade() = default;

  double price;
  int volume;
  int updateaction;
    std::string symbol;
  friend std::ostream &operator<<(std::ostream &os, const Trade &trade) {

    std::string update;
    switch(trade.updateaction) {
        case 0: update = "New"; break;
        case 1: update = "Change"; break;
        case 2: update = "Delete"; break;
    }

    os << trade.symbol << " Trade >>  price: " << trade.price << " volume: " << trade.volume<< " update: " << update;
    return os;
  }
};

