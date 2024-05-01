#include <fstream>
#include "fmt/core.h"
#include "fmt/ranges.h"
#include "functions.hpp"

auto f_encrypt(std::filesystem::path const& file, std::string const& message) -> int{
    if(file.extension() == ".bmp"){
        {
            auto sizeOfImage = BMP::getSizeOfImage(file);
            fmt::println("{}",BMP::getNumOfPadding(sizeOfImage));
        }


        {
//        fmt::println("{}", numberOfBytes);
//        fmt::println("{}", sizeof(char));
        fmt::println("{}", sizeof(std::byte));
//        fmt::println("{}", sizeof(char8_t));
//        fmt::println("{}", sizeof(int));
        fmt::println("{}", sizeof(bool));
        }

        {
            auto buffer = std::vector<std::byte>(4);
            auto input = std::ifstream(file, std::ios_base::binary);
            input.seekg(0xA);
            input.read(reinterpret_cast<char*>(buffer.data()),4);
            auto num = (int(buffer[0]) << 0) | (int(buffer[1]) << 8) | (int(buffer[2]) << 16) | (int(buffer[3]) << 24);
            fmt::println("{}", num);
            input.seekg(num);
            buffer = std::vector<std::byte>(3);
            input.read(reinterpret_cast<char*>(buffer.data()),3);
            fmt::println("{}",buffer);


        }


    } else {

    }

    fmt::println("ENCRYPT!");
    return 12;
}

auto f_decrypt(std::filesystem::path const& file) -> int{
    fmt::println("DECRYPT!");
    return 15;
}

auto f_check(std::filesystem::path const& file, std::string const& message) -> int{
    auto numberOfChar = message.size();
    auto sizeOfImage = sizeOfImageHelper(file);
    auto numberOfBytes = sizeOfImage.width * sizeOfImage.height / 8;
    if(numberOfChar <= numberOfBytes){
        fmt::println("A message can be encrypt in file {}!", file.filename().string());
        return 0;
    }
    else {
        fmt::println("A message can not be encrypt in file {}!", file.filename().string());
        return 8;
    }

}