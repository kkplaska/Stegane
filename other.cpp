#include <fstream>
#include "functions.hpp"
#include "fmt/core.h"

namespace PPM {
    //Pozostałość po PNG, zmienić na PPM
    auto getSizeOfImage(std::filesystem::path const& file) -> SizeOfImage{
        auto size = std::vector<int>(2);
        auto buffer = std::vector<char>(8);
        auto input = std::fstream(file, std::ios::binary | std::ios::in);
        input.seekg(0x10); // Skok do 16 bitu
        input.read(&buffer[0],8);
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
        auto buffer = std::vector<char>(8);
        auto input = std::fstream(file, std::ios::binary | std::ios::in);
        input.seekg(0x12); // Skok do 18 bitu
        input.read(&buffer[0],8);
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

    auto getNumOfPadding(SizeOfImage const& soi) -> int{
        return soi.width % 4;
    }
}

auto sizeOfImageHelper(std::filesystem::path const& file) -> SizeOfImage{
    if(file.extension() == ".bmp") {
        return BMP::getSizeOfImage(file);
    }
    else if (file.extension() == ".ppm") {
        return PPM::getSizeOfImage(file);
    }
    return SizeOfImage{0,0};
};