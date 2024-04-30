#include <fstream>
#include "functions.hpp"
#include "fmt/core.h"

auto getSizeOfImage(std::filesystem::path const& file) -> std::vector<int>{
    auto size = std::vector<int>(2);
    auto buffer = std::vector<std::byte>(8);
    // Musi być ifstream, istream nie działa
    auto input = std::ifstream(file, std::ios_base::binary);
    if(file.extension().string() == ".bmp"){ // BMP
        input.seekg(18); // Pominięcie pierwszych 18 bitów
        input.read(reinterpret_cast<char*>(buffer.data()),8);
        //Width
        size.at(0) =   (int(buffer[0]) << 0)
                        + (int(buffer[1]) << 8)
                        + (int(buffer[2]) << 16)
                        + (int(buffer[3]) << 24);
        //Height
        size.at(1) =   (int(buffer[4]) << 0)
                        + (int(buffer[5]) << 8)
                        + (int(buffer[6]) << 16)
                        + (int(buffer[7]) << 24);
    } else { //PNG
        input.seekg(16); // Pominięcie pierwszych 16 bitów
        input.read(reinterpret_cast<char*>(buffer.data()),8);
        //Width
        size.at(0) =   (int(buffer[0]) << 24)
                        + (int(buffer[1]) << 16)
                        + (int(buffer[2]) << 8)
                        + (int(buffer[3]) << 0);
            //Height
        size.at(1) =   (int(buffer[4]) << 24)
                        + (int(buffer[5]) << 16)
                        + (int(buffer[6]) << 8)
                        + (int(buffer[7]) << 0);
    }
    return size;
}