#include "OrderBook.h"

constexpr int MAX_BOOK_SIZE = 10;

OrderBook::OrderBook() {
    bids_.reserve(MAX_BOOK_SIZE);
    asks_.reserve(MAX_BOOK_SIZE);
}

bool OrderBook::empty() const {
    return bids_.empty() and asks_.empty();
}

void OrderBook::add_bid(int level, float price, int quantity) {

    if(level-1 > bids_.size())
        return;

    bids_.insert(bids_.begin()+level-1, {price, quantity});
}

void OrderBook::update_bid(int level, float price, int quantity) {

    if(level > bids_.size())
        return;

    auto& price_level = bids_[level-1];
    if(price_level.price == price)
        price_level.quantity = quantity;
    else
        std::cout << "Error: Price mismatch on update request." << '\n';
}

void OrderBook::delete_bid(int level, float price) {
    if(level > bids_.size())
        return;

    auto it = bids_.begin() + level - 1;
    if(it->price == price)
        bids_.erase(it);
    else
        std::cout << "Error: Price mismatch on delete request." << '\n';
}

void OrderBook::delete_bid_from(int level) {
    if(level > bids_.size())
        return;
    bids_.erase(bids_.begin()+level-1, bids_.end());
}

void OrderBook::delete_bid_thru(int level) {
    if(level == 1)
        bids_.clear();
    else
        std::cout << "Error: delete thru expects level = 1";
}

void OrderBook::add_ask(int level, float price, int quantity) {
    if(level-1 > asks_.size())
        return;
    asks_.insert(asks_.begin()+level-1, {price, quantity});
}

void OrderBook::update_ask(int level, float price, int quantity) {

    if(level > asks_.size())
        return;

    auto& price_level = asks_[level-1];
    if(price_level.price == price)
        price_level.quantity = quantity;
    else
        std::cout << "Error: Price mismatch on update request." << '\n';
}

void OrderBook::delete_ask(int level, float price) {
    if(level > asks_.size())
        return;
    auto it = asks_.begin() + level - 1;
    if(it->price == price)
        asks_.erase(it);
    else
        std::cout << "Error: Price mismatch on delete request." << '\n';
}

void OrderBook::delete_ask_from(int level) {
    if(level > asks_.size())
        return;
    asks_.erase(asks_.begin()+level-1, asks_.end());
}

void OrderBook::delete_ask_thru(int level) {
    if(level == 1)
        asks_.clear();
    else
        std::cout << "Error: delete thru expects level = 1";
}

std::ostream& operator<<(std::ostream &os, const OrderBook &book) {

    os << "BidV\t\tPrice\t\tAskV" << '\n';
    os << "--------------------------------------" << '\n';

    for(auto it = book.asks_.rbegin(); it != book.asks_.rend(); ++it) {
        os << "\t\t"
           << it->price
           << "\t\t"
           << it->quantity
           << '\n';
    }

    os << "--------------------------------------" << '\n';

    for(auto it = book.bids_.begin(); it != book.bids_.end(); ++it) {
        os << it->quantity
           << "\t\t"
           << it->price
           << '\n';
    }

    return os;
}

void OrderBook::clear() {
	bids_.clear();
	asks_.clear();
}
