#include "fmt/core.h"
#include "functions.hpp"

auto f_encrypt(std::string const& pathToFile, std::string const& message) -> int{
    fmt::println("ENCRYPT!");
    return 12;
}

auto f_decrypt(std::string const& pathToFile) -> int{
    fmt::println("DECRYPT!");
    return 15;
}

auto f_check(std::string const& pathToFile, std::string const& message) -> int{
    auto file = std::filesystem::path(pathToFile);
    auto numberOfChar = message.size();
    auto compar = int();
    {
        auto sizeOfImage = getSizeOfImage(file);
        auto numberOfBytes = sizeOfImage.at(0) * sizeOfImage.at(1) / 8;
        compar = numberOfBytes;

//        fmt::println("{}", numberOfBytes);
//        fmt::println("{}", sizeof(char));
//        fmt::println("{}", sizeof(std::byte));
//        fmt::println("{}", sizeof(char8_t));
//        fmt::println("{}", sizeof(int));
    }
    fmt::println("{} <> {}", numberOfChar, compar);

    if(numberOfChar <= compar){
        fmt::println("A message can be encrypt in file {}!", file.filename().string());
        return 0;
    }
    else {
        fmt::println("A message can not be encrypt in file {}!", file.filename().string());
        return 8;
    }

}