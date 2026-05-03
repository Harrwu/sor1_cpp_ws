#pragma once
#include "include/orderpool.h"
#include <cstdint>
#include <unordered_map>

struct Order{
    uint64_t order_id;
    uint64_t price;
    uint32_t quanity;
    bool is_buy;

    // Double linked list for fast delete
    Order* next = nullptr;
    Order* prev = nullptr;
};

class orderbook{
private:
    Orderpool memory_pool_;
    std::unordered_map<uint64_t, Order*> order_map_;
public:
    orderbook(Orderpool &pool): memory_pool_(pool){};

    void add_order(uint64_t id, uint64_t price, uint32_t qty, bool is_buy);
    void cancel_order(uint64_t id, uint64_t);
    void execute_trade();
};