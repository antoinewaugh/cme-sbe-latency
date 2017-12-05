#pragma once

#include "DepthList.h"
#include <iostream>

class DepthBook {

  DepthList bids_, asks_;

public:
  DepthBook();
  const DepthList Bids() const;
  const DepthList Asks() const;
  inline bool Empty() const { return bids_.Empty() && asks_.Empty(); }
  void Clear();
  void AddBid(int level, float price, int quantity);
  void UpdateBid(int level, float price, int quantity);
  void DeleteBid(int level, float price);
  void DeleteBidFrom(int level);
  void DeleteBidThru();
  void AddAsk(int level, float price, int quantity);
  void UpdateAsk(int level, float price, int quantity);
  void DeleteAsk(int level, float price);
  void DeleteAskFrom(int level);
  void DeleteAskThru();
  friend std::ostream &operator<<(std::ostream &os, const DepthBook &book);
};
