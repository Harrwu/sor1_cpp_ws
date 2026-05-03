#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>

int main(){
    std::ifstream file ("../data/data_feeds_20260427_20260427_IEXTP1_DEEP1.0.pcap", std::ios::binary | std::ios::ate);
    if (!file.is_open()){
        std::cerr << "Cannot Open file\n";
        return 0;
    }

    const size_t CHUNCK_SIZE = 4096;
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(CHUNCK_SIZE);
    while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0){
        for (int i = 0; i < buffer.size(); i ++) std::cout << buffer[i] << "\n";
    }

    return 0;
}