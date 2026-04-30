#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <array>

class memory{
private:
    size_t m_size;
    std::vector<uint8_t> raw;
public:

    memory(size_t size);

    void write32(uint32_t address, uint32_t value);
    void write16(uint32_t address, uint16_t value);
    void write8(uint32_t address, uint8_t value);
    
    void load_program (const std::string &filename);

    uint32_t read32(uint32_t address);
    uint16_t read16(uint32_t address);
    uint8_t read8(uint32_t address);

    void coutRaw(std::ofstream& rawO);
};