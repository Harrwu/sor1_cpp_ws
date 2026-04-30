#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "memory.h" 
#include "cpu.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " {FILE ADDR}" << std::endl;
        return 0;
    }
    bool debug; 
    {
        std::string pass; 
        std::cin >> pass;
        if (pass == "true" || pass == "1") debug = true;
        else debug =  false;
    }

    size_t m_size = 65536;
    memory ram(m_size);
    CPU cpu(&ram, m_size, debug);

    try {
        cpu.load_program(argv[1]);
        cpu.write_reg(2, 1024);
        cpu.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
