#pragma once

#include <iostream>
#include <vector>

class OrderBook {

  struct PriceLevel {
    float price;
    int quantity;
  };

  std::vector<PriceLevel> bids_, asks_;

public:
  OrderBook();
  inline bool Empty() const;
  void Clear();
  void AddBid(int level, float price, int quantity);
  void UpdateBid(int level, float price, int quantity);
  void DeleteBid(int level, float price);
  void DeleteBidFrom(int level);
  void DeleteBidThru(int level);
  void AddAsk(int level, float price, int quantity);
  void UpdateAsk(int level, float price, int quantity);
  void DeleteAsk(int level, float price);
  void DeleteAskFrom(int level);
  void DeleteAskThru(int level);
  friend std::ostream &operator<<(std::ostream &os, const OrderBook &book);
};
