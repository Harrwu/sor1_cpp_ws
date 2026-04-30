#pragma once
#include "memory.h"
#include <memory>
#include <iostream>
#include <cstdint>
#include <vector>
#include <array>

class CPU{
private:
    std::array<uint32_t, 32> reg = {0};
    uint32_t pc = 0;
    uint32_t m_size;
    memory* ram;
    bool is_running = true;
    bool debug;

public:
    CPU (memory* m_ram, const size_t m_size, const   bool m_debug): ram(m_ram), m_size(this->m_size), debug(m_debug){}

    ~CPU(){};

    

    void write_reg(uint32_t index,uint32_t value);
    uint32_t read_reg(uint32_t index);
    uint32_t fetch();
    uint32_t fetch_sp();
    void print_state();
    void load_program (const std::string &filename);

    void decode_execute(uint32_t instruction);
    void run();

    inline uint32_t get_pc(){ return pc;}
};