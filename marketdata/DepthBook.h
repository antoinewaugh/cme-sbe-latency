#pragma once

#include "DepthList.h"
#include <iostream>
#include <string>

class DepthBook {

  DepthList bids_, asks_;
  std::string security_;
  uint32_t securityid_;
public:
  DepthBook(std::string security, uint32_t securityid, uint32_t size):security_(security), securityid_(securityid), bids_(true, size), asks_(false, size) {}
  const DepthList Bids() const;
  const DepthList Asks() const;
  inline bool Empty() const { return bids_.Empty() && asks_.Empty(); }
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
  friend std::ostream &operator<<(std::ostream &os, DepthBook const& book);
};
