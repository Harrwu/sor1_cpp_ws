#include "memory.h"
#include <vector>
#include <memory>
#include <cstdint>
#include <array>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>

memory::memory(size_t size): m_size(size), raw(size, 0){}

void memory::write32(uint32_t address, uint32_t value){
    if (address + 3 >= m_size) return;
    for (int i = 0; i < 4; i++) 
        raw[address + i] = (value >> (i * 8)) & 0xFF;
}

void memory::write16(uint32_t address, uint16_t value){
    if (address + 1 >= m_size) return;
    for (int i = 0; i < 2; i ++){
        raw[address + i] = (value >> (i * 8) & 0xFF);
    }
}

void memory::write8(uint32_t address, uint8_t value){
    if (address >= m_size) return;
    raw[address] = value & 0xFF;
}

uint32_t memory::read32(uint32_t address){
    uint32_t res = 0;
    if (address + 3 >= m_size) return 0 ;
    for (size_t i = 0; i < 4; i ++)
        res |= (static_cast<uint32_t>(raw[address + i]) << 8 * i);
    return res;
}

uint8_t memory::read8(uint32_t address){
    uint32_t res = 0;
    if (address >= m_size) return 0 ;
    
    return static_cast<uint32_t>(raw[address]);
}
uint16_t memory::read16(uint32_t address){
    uint32_t res = 0;
    if (address + 1 >= m_size) return 0 ;
    for (size_t i = 0; i < 2; i ++)
        res |= (static_cast<uint32_t>(raw[address + i]) << 8 * i);
    return res;
}

void memory::coutRaw(std::ofstream& rawO){
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    if (!rawO.is_open()){
        std::cout << "CANNOT OPEN FILE raw.txt\n";
        return;
    }

    rawO << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << "\n";
    for (int i = 0; i < m_size; i ++){
        rawO << std::dec << i << ": " << "0x" << std::hex <<static_cast<int>(raw[i]) << "\n";
    }
    rawO.close();
}