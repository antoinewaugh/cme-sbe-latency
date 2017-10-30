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
    inline bool empty() const;
    void clear();
    void add_bid(int level, float price, int quantity);
    void update_bid(int level, float price, int quantity);
    void delete_bid(int level, float price);
    void delete_bid_from(int level);
    void delete_bid_thru(int level);
    void add_ask(int level, float price, int quantity);
    void update_ask(int level, float price, int quantity);
    void delete_ask(int level, float price);
    void delete_ask_from(int level);
    void delete_ask_thru(int level);
    friend std::ostream& operator<<(std::ostream& os, const OrderBook& book);
};


