#include "DepthBook.h"

DepthBook::DepthBook() : bids_(true), asks_(false) {}

void DepthBook::AddBid(int level, float price, int quantity) {
  bids_.Add(level, price, quantity);
}

void DepthBook::UpdateBid(int level, float price, int quantity) {
  bids_.Update(level, price, quantity);
}

void DepthBook::DeleteBid(int level, float price) {
  bids_.Delete(level, price);
}

void DepthBook::DeleteBidFrom(int level) { bids_.DeleteFrom(level); }

void DepthBook::DeleteBidThru() { bids_.DeleteThru(); }

void DepthBook::AddAsk(int level, float price, int quantity) {
  asks_.Add(level, price, quantity);
}

void DepthBook::UpdateAsk(int level, float price, int quantity) {
  asks_.Update(level, price, quantity);
}

void DepthBook::DeleteAsk(int level, float price) {
  asks_.Delete(level, price);
}

void DepthBook::DeleteAskFrom(int level) { asks_.DeleteFrom(level); }

void DepthBook::DeleteAskThru() { asks_.DeleteThru(); }

void DepthBook::Clear() {
  bids_.Clear();
  asks_.Clear();
}

const DepthList DepthBook::Bids() const { return bids_; }

const DepthList DepthBook::Asks() const { return asks_; }

std::ostream &operator<<(std::ostream &os, const DepthBook &book) {

  os << "BidV\t\tPrice\t\tAskV" << '\n';
  os << "--------------------------------------" << '\n';

  os << book.Asks();

  os << "--------------------------------------" << '\n';

  os << book.Bids();

  return os;
}
