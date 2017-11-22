#pragma once

#include <iostream>
#include <vector>

constexpr int MAX_BOOK_SIZE = 10;

class DepthList {

private:
  struct PriceLevel {
    float price;
    int quantity;
  };

  bool isbid_;
  std::vector<PriceLevel> levels_;

public:
  DepthList(bool isbid);
  inline bool Empty() const { return levels_.empty(); }
  void Clear();
  void Add(int level, float price, int quantity);
  void Update(int level, float price, int quantity);
  void Delete(int level, float price);
  void DeleteFrom(int level);
  void DeleteThru();
  friend std::ostream &operator<<(std::ostream &os, const DepthList &book);
};
