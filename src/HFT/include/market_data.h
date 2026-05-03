#pragma once
#include "order.h"

struct PriceLevel{
    uint64_t price;
    Order* head = nullptr;
    Order* tail = nullptr;

    uint64_t total_volume = 0;
};