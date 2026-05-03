#pragma once
#include "include/order.h"

class Orderpool{
private:

    Order* pool_array_;
    uint32_t capacity_;

public:
    Orderpool(uint32_t max_orders);
    ~Orderpool();

    Order* allocate_order(); // Function finds a free spot in the pool_array_ and returns the address
    void free_order(); // Function removes the data inside an address 

};