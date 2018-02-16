#pragma once

#include <iostream>
#include <vector>

class DepthList {

private:
  struct PriceLevel {
    float price;
    int quantity;
  };

  bool isbid_;
  uint32_t size_;
  std::vector<PriceLevel> levels_;

public:
  DepthList(bool isbid, uint32_t size);
  inline bool Empty() const { return levels_.empty(); }
  void Clear();
  PriceLevel const& Get(int level) const { return levels_[level];}
  size_t Size() {return levels_.size();}
  bool Has(int level) {return levels_.size() > level; }
  void Add(int level, float price, int quantity);
  void Update(int level, float price, int quantity);
  void Delete(int level, float price);
  void DeleteFrom(int level);
  void DeleteThru(int level);
  std::string ToString();
  friend std::ostream &operator<<(std::ostream &os, const DepthList &book);
};
