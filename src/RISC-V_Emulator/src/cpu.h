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

    /*
        current privilege is the assigment of rights and permission to specific
        user these rights are:

        Machine Mode (M-mode) = 3; access to all memory and regiesterrs. When booting on the mode is set to mahcine mode.
        Supervisor Mode (S-mode) = 1;  For operating system's kernel. Enough power to manage memory and schedule task but restricted from touching the most deepest hardware configs
        User Mode (U-Mode) = 0; Programs are heavily restricted. Can only do math, logic and standard write and read to the memory
    */
    uint8_t current_privilege = 3;

public:
    CPU (memory* m_ram, const size_t m_size, const   bool m_debug): ram(m_ram), m_size(this->m_size), debug(m_debug){}

    ~CPU(){};

    

    void write_reg(uint32_t index, uint32_t value); // writes to register
    uint32_t read_reg(uint32_t index); // reads register value
    void print_state(); // prints current reg
    void load_program (const std::string &filename); // loads binary bin files and loads into ram


    // Fetch decode and execute cycle
    uint32_t fetch();
    void decode_execute(uint32_t instruction);

    void run();
};