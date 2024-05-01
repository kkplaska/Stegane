#include <fstream>
#include "functions.hpp"
#include "fmt/core.h"

namespace PNG {
    auto getSizeOfImage(std::filesystem::path const& file) -> SizeOfImage{
        auto size = std::vector<int>(2);
        auto buffer = std::vector<std::byte>(8);
        // Musi być ifstream, istream nie działa
        auto input = std::ifstream(file, std::ios_base::binary);
        input.seekg(0x10); // Skok do 16 bitu
        input.read(reinterpret_cast<char*>(buffer.data()),8);
        //Width
        auto width = int((int(buffer[0]) << 24)
                       | (int(buffer[1]) << 16)
                       | (int(buffer[2]) << 8)
                       | (int(buffer[3]) << 0));
        //Height
        auto height = int((int(buffer[4]) << 24)
                       | (int(buffer[5]) << 16)
                       | (int(buffer[6]) << 8)
                       | (int(buffer[7]) << 0));
        return SizeOfImage{width, height};
    }
}

namespace BMP{
    auto getSizeOfImage(std::filesystem::path const& file) -> SizeOfImage{
        auto size = std::vector<int>(2);
        auto buffer = std::vector<std::byte>(8);
        // Musi być ifstream, istream nie działa
        auto input = std::ifstream(file, std::ios_base::binary);
        input.seekg(0x12); // Skok do 18 bitu
        input.read(reinterpret_cast<char*>(buffer.data()),8);
        //Width
        auto width = int((int(buffer[0]) << 0)
                       | (int(buffer[1]) << 8)
                       | (int(buffer[2]) << 16)
                       | (int(buffer[3]) << 24));
        //Height
        auto height = int((int(buffer[4]) << 0)
                        | (int(buffer[5]) << 8)
                        | (int(buffer[6]) << 16)
                        | (int(buffer[7]) << 24));
        return SizeOfImage{width, height};
    }

    // BMP - number of bits per pixel
    auto getNumBPP(std::filesystem::path const& file) -> int{
        auto buffer = std::vector<std::byte>(2);
        auto input = std::ifstream(file, std::ios_base::binary);
        input.seekg(0x1C);
        input.read(reinterpret_cast<char*>(buffer.data()),2);
        auto num = (int(buffer[0]) << 0) | (int(buffer[1]) << 8);
        return num;
    }

    auto getNumOfPadding(SizeOfImage const& soi) -> int{
        return soi.width % 4;
    }
}
