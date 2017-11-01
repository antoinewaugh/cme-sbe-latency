#include "OrderBook.h"

constexpr int MAX_BOOK_SIZE = 10;

OrderBook::OrderBook() {
  bids_.reserve(MAX_BOOK_SIZE);
  asks_.reserve(MAX_BOOK_SIZE);
}

bool OrderBook::Empty() const { return bids_.empty() and asks_.empty(); }

void OrderBook::AddBid(int level, float price, int quantity) {
  if (level - 1 <= bids_.size())
    bids_.insert(bids_.begin() + level - 1, {price, quantity});
  else
    std::cerr << "Add bid out of range." << '\n';
}

void OrderBook::UpdateBid(int level, float price, int quantity) {
  if (level - 1 < bids_.size()) {
    auto &price_level = bids_[level - 1];
    if (price_level.price == price)
      price_level.quantity = quantity;
    else
      std::cerr << "Price mismatch on update request." << '\n';
  } else {

    std::cerr << "Update bid level out of range." << '\n';
  }
}

void OrderBook::DeleteBid(int level, float price) {
  if (level - 1 < bids_.size()) {

    auto it = bids_.begin() + level - 1;
    if (it->price == price)
      bids_.erase(it);
    else
      std::cerr << "Price mismatch on delete request." << '\n';
  } else {
    std::cerr << "Delete bid level out of range." << '\n';
  }
}

void OrderBook::DeleteBidFrom(int level) {
  if (level - 1 < bids_.size())
    bids_.erase(bids_.begin() + level - 1, bids_.end());
  else
    std::cerr << "Delete bid from level out of range." << '\n';
}

void OrderBook::DeleteBidThru(int level) {
  if (level == 1)
    bids_.clear();
  else
    std::cerr << "Delete thru expects level = 1";
}

void OrderBook::AddAsk(int level, float price, int quantity) {
  if (level - 1 <= asks_.size())
    asks_.insert(asks_.begin() + level - 1, {price, quantity});
  else
    std::cerr << "Add ask level out of range." << '\n';
}

void OrderBook::UpdateAsk(int level, float price, int quantity) {

  if (level - 1 < asks_.size()) {

    auto &price_level = asks_[level - 1];
    if (price_level.price == price)
      price_level.quantity = quantity;
    else
      std::cerr << "Price mismatch on update request." << '\n';
  } else {
    std::cerr << "Update ask level out of range." << '\n';
  }
}

void OrderBook::DeleteAsk(int level, float price) {
  if (level - 1 < asks_.size()) {
    auto it = asks_.begin() + level - 1;
    if (it->price == price)
      asks_.erase(it);
    else
      std::cerr << "Price mismatch on delete request." << '\n';
  } else {
    std::cerr << "Delete ask out of range." << '\n';
  }
}

void OrderBook::DeleteAskFrom(int level) {
  if (level - 1 < asks_.size())
    asks_.erase(asks_.begin() + level - 1, asks_.end());
  else
    std::cerr << "Delete ask from out of range." << '\n';
}

void OrderBook::DeleteAskThru(int level) {
  if (level == 1)
    asks_.clear();
  else
    std::cerr << "Error: delete thru expects level = 1";
}

std::ostream &operator<<(std::ostream &os, const OrderBook &book) {

  os << "BidV\t\tPrice\t\tAskV" << '\n';
  os << "--------------------------------------" << '\n';

  for (auto it = book.asks_.rbegin(); it != book.asks_.rend(); ++it) {
    os << "\t\t" << it->price << "\t\t" << it->quantity << '\n';
  }

  os << "--------------------------------------" << '\n';

  for (auto it = book.bids_.begin(); it != book.bids_.end(); ++it) {
    os << it->quantity << "\t\t" << it->price << '\n';
  }

  return os;
}

void OrderBook::Clear() {
  bids_.clear();
  asks_.clear();
}
