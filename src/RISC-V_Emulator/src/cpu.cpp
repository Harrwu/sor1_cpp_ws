#include "memory.h"
#include "cpu.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include <array>
#include <fstream>

void CPU::write_reg(uint32_t index, uint32_t value){
    if (index == 0) return;
    reg[index] = value;
}

void CPU::print_state(){
    std::cout << "PC: " << pc << "\n";
    for (int i = 0; i < 32; i ++){
        if ((int)reg[i] != 0) std::cout << "x" << std::dec << i << ": 0x" << std::hex << (int)reg[i] << "\n";
    }
}

void CPU::decode_execute(uint32_t instruction){
    uint8_t opcode = instruction & 0x7f;

    // R-type Register
    if (opcode == 0x33){
        uint8_t rd = (instruction >> 7) & 0x1f;
        uint8_t rs1I = (instruction >> 15) & 0x1f;
        uint8_t rs2I = (instruction >> 20) & 0x1f;
        uint8_t func3 = (instruction >> 12) & 0x07;
        uint8_t func7 = (instruction >> 25) & 0x7f;

        uint32_t rs1 = read_reg(rs1I);
        uint32_t rs2 = read_reg(rs2I);
        uint32_t shift = rs2 & 0x1F;

        if (func7 == 0x00){
            
            if (func3 == 0x0) write_reg(rd, rs1 + rs2); // ADD adds rs1 and rs2 to rd
            else if (func3 == 0x4) write_reg(rd, rs1 ^ rs2); // XOR Bitwise XOR rs1 and rs2 rd           
            else if (func3 == 0x6) write_reg(rd, rs1 | rs2); // OR Bitwise Or rs1 and rs2 to rd
            else if (func3 == 0x7) write_reg(rd, rs1 & rs2); // AND Bitwise AND rs1 and rs2 to rd
            else if (func3 == 0x1) write_reg(rd, rs1 << shift); // SLL Shifts rs1 left by rs2 to rd
            else if (func3 == 0x5) write_reg(rd, rs1 >> shift); // SLR Shifts rs1 right by rs2 to rd
            else if (func3 == 0x2) write_reg(rd, (static_cast<int32_t>(rs1) < static_cast<int32_t>(rs2)) ? 1 : 0); // SLT if rs1 < rs2 than rd = 1 else rd = 0
            else if (func3 == 0x3) write_reg(rd, (rs1 > rs2) ? 1: 0); // SLTU if Unsigned rs1 < Unsigned rs2 than rd = 1 else rd = 0
            
        }else if (func7 == 0x20){
            if (func3 == 0x5) write_reg(rd, static_cast<int32_t>(rs1) >> shift); //
            if (func3 == 0x0) write_reg(rd, rs1 - rs2); // SUB rs1 subtracts rs2 to regiester rd
        }
    }
    // I type IMMEDIATE
    else if (opcode == 0x13){
        uint8_t rd = (instruction >> 7) & 0x1f;
        uint8_t rs1 = (instruction >> 15) & 0x1f;
        int32_t immediate = static_cast<int32_t>(instruction) >> 20;
        uint8_t func3 = (instruction >> 12) & 0x07;

        uint32_t rs1V = read_reg(rs1);
        uint32_t shift = immediate & 0x1F;

        if (func3 == 0x1) { // SLLI
        write_reg(rd, rs1V << shift);
        } 
        else if (func3 == 0x5) { 
            if ((instruction >> 30) & 0x1) 
                write_reg(rd, static_cast<int32_t>(rs1V) >> shift);
            else 
                write_reg(rd, rs1V >> shift);
        }
        else{
            if (func3 == 0x0) write_reg (rd, rs1V + immediate);
            else if (func3 == 0x4) write_reg(rd, rs1V ^ immediate);
            else if (func3 == 0x6) write_reg(rd, rs1V | immediate);
            else if (func3 == 0x7) write_reg(rd, rs1V & immediate);
            else if (func3 == 0x2) write_reg(rd, (static_cast<int32_t>(rs1V) < immediate) ? 1: 0); //SLTI
            else if (func3 == 0x3) write_reg(rd, (rs1V < static_cast<uint32_t>(immediate)) ? 1: 0); //SLTIU
        }
    }

    // L-type LOAD
    else if (opcode == 0x03) {
        uint8_t rd = (instruction >> 7) & 0x1f;
        uint8_t func3 = (instruction >> 12) & 0x07;
        uint8_t rs1 = (instruction >> 15) & 0x1f;
        int32_t immediate = static_cast<int32_t>(instruction) >> 20;
        uint32_t target_addr = static_cast<int32_t>(read_reg(rs1)) + immediate; 

        if (func3 == 0x0) write_reg(rd, static_cast<int32_t>(static_cast<int8_t>(ram->read32(target_addr) & 0xFF)));
        else if (func3 == 0x1) write_reg(rd, static_cast<int32_t>(static_cast<int16_t>(ram->read32(target_addr) & 0xFFFF)));
        else if (func3 == 0x2) write_reg(rd, ram->read32(target_addr));
        else if (func3 == 0x4) write_reg(rd, static_cast<uint32_t>(ram->read32(target_addr) & 0xFF));
        else if (func3 == 0x5) write_reg(rd, static_cast<uint32_t>(ram->read32(target_addr) & 0xFFFF));
    }
    
    // S-type STORE
    else if (opcode == 0x23){
        uint8_t rs1 = (instruction >> 15) & 0x1f;
        uint8_t rs2 = (instruction >> 20) & 0x1F;
        uint8_t func3 = (instruction >> 12) & 0x07;
        int32_t immediate = (((instruction >> 25) & 0x7F) << 5) | ((instruction >> 7) & 0x1F);
        if (immediate & 0x800) immediate |= 0xFFFFF000;
        uint32_t target_addr = static_cast<int32_t>(read_reg(rs1)) + immediate; 
        if (func3 == 0x0) ram->write8(target_addr, static_cast<uint8_t>(reg[rs2]));
        else if (func3 == 0x1) ram->write16(target_addr, static_cast<uint16_t>(reg[rs2]));
        else if (func3 == 0x2) ram->write32(target_addr, reg[rs2]);     
    }

    // B-type Branch
    else if (opcode == 0x63){
        uint8_t rs1I = (instruction >> 15) & 0x1F;
        uint8_t rs2I = (instruction >> 20) & 0x1F;
        uint8_t func3 = (instruction >> 12) & 0x07;
        uint32_t rs1 = read_reg(rs1I);
        uint32_t rs2 = read_reg(rs2I);
        int32_t immediate;
        {
            uint32_t b31 = (instruction >> 31) & 0x1;
            uint32_t b7 = (instruction >> 7) & 0x1;
            uint32_t b25 = (instruction >> 25) & 0x3F;
            uint32_t b8 = (instruction >> 8) & 0xF;

            immediate = (b31 << 12) | (b7 << 11) | (b25 << 5) | (b8 << 1);
        }
        if (immediate & 0x1000) immediate |= 0xFFFFE000;

        uint32_t jump_target = (pc - 4) + immediate;
        if (func3 == 0x0 && rs1 == rs2) pc = jump_target;
        else if (func3 == 0x1 && rs1 != rs2) pc += immediate;
        else if (func3 == 0x4 && static_cast<int32_t>(rs1) < static_cast<int32_t>(rs2)) pc = jump_target;
        else if (func3 == 0x5 && static_cast<int32_t>(rs1) >= static_cast<int32_t>(rs2)) pc = jump_target;
        else if (func3 == 0x6 && rs1 < rs2) pc = jump_target;
        else if (func3 == 0x7 && rs1 >= rs2) pc = jump_target;
    }

    else if (opcode == 0x67){
        uint8_t rd = (instruction >> 7) & 0x1f;
        uint8_t rs1 = (instruction >> 15) & 0x1f;

        int32_t immediate = static_cast<int32_t>(instruction) >> 20;
        uint32_t return_addr = pc;
        pc = (read_reg(rs1) + immediate) & ~1;
        write_reg(rd, return_addr);
    }

    // JAL
    else if (opcode == 0x6F){
        uint8_t rd = (instruction >> 7) & 0x1f;
        int32_t immediate;
        {
            uint32_t imm_20    = (instruction >> 31) & 0x1;
            uint32_t imm_19_12 = (instruction >> 12) & 0xFF;
            uint32_t imm_11    = (instruction >> 20) & 0x1;
            uint32_t imm_10_1  = (instruction >> 21) & 0x3FF;
            immediate = (imm_20 << 20) | (imm_19_12 << 12) | (imm_11 << 11) | (imm_10_1 << 1);

            if (immediate & 0x100000) immediate |= 0xFFE00000;
        }

        

        write_reg(rd, pc);
        pc = (pc - 4) + immediate;
    }

    else if (opcode == 0x37){
        int32_t immediate = static_cast<int32_t>(instruction) >> 12;
        uint8_t rd = (instruction >> 7) & 0x1F;
        write_reg(rd, immediate << 12);
    }

    else if (opcode == 0x17){
        int32_t immediate = static_cast<int32_t>(instruction) >> 12;
        uint8_t rd = (instruction >> 7) & 0x1F;
        write_reg(rd, (pc - 4) + (immediate << 12));
    }

    //ECALL
    else if (opcode == 0x73) {
        uint32_t systemcall_id = read_reg(17);
        if (debug) std::cout << "ECALL CALLED\n";
        if (debug) std::cout << systemcall_id << "\n";

        if (systemcall_id == 64){ // Write
            if (debug) std::cout << "PRINT\n";
            uint32_t fd = read_reg(10); // FD 1 write 2 error
            uint32_t addr = read_reg(11); // MEMORY ADDRESS of string
            uint32_t len = read_reg(12); // LENGTH of string

            for (uint32_t i = 0; i < len; i++) {
                char c = (char)ram->read8(addr + i);
                if (c == '\0') break;
                std::cout << c;
            }
            std::cout << "\n";
        }else if (systemcall_id == 63){ // Read
            uint32_t fd = read_reg(10); // FD
            uint32_t addr = read_reg(11); // MEMORY ADDRESS of string
            uint32_t len = read_reg(12); // Max length

            std::string s; std::getline(std::cin, s);
            s += "\n";

            uint32_t bytes_to_write = (s.size() > len) ? len: s.size();

            for (uint32_t i = 0; i < s.size(); i++) {
                ram->write8(addr + i, (uint8_t)s[i]);
            }
            ram->write8(addr + bytes_to_write, '\0');
            std::cout << "\n";
            write_reg(10, bytes_to_write);
        }else if (systemcall_id == 88) { // Custom, calls opengl
            uint8_t window_s = read_reg(11);
            
        }
        
        
        else if (systemcall_id == 93){
            is_running = false;
        }else{
            if (debug) std::cout << "Unknown Syscall ID: " << systemcall_id << " at PC: 0x" << std::hex << (pc - 4) << std::endl;
            is_running = false;
        }
    }

    else{
        std::cout << "UNKOWN OPCODE: " << opcode << "\n";
        is_running = false;
    }
}

uint32_t CPU::read_reg(uint32_t index){
    if (index >= 32) return 0;
    return reg[index];
}


uint32_t CPU::fetch(){
    auto res = ram->read32(pc);
    pc += 4;
    return res;
}

void CPU::run(){
    std::ofstream rawO("raw.txt", std::ios::trunc);
    while (is_running){
        decode_execute(fetch());
        if(debug) print_state();
    }

    ram->coutRaw(rawO);
}

void CPU::load_program(const std::string &filename){
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        is_running = false;
    }else{

        // Gets file size by moving pointer to last character and getting the index.
        std::streamsize size = file.tellg();
        // Points the pointer back to the first index
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> buffer(size);
        if (file.read(reinterpret_cast<char*>(buffer.data()), size)){
            for (int i = 0; i < buffer.size(); i ++) ram->write8(i, buffer[i]);
            std::cout << "Successfully loaded " << size << " bytes." << std::endl;
        }
    }
}