#include <filesystem>
#include <fstream>
#include <vector>
#include "functions.hpp"
#include "fmt/core.h"

auto f_info(std::string const& pathToFile) -> int{
    auto file = std::filesystem::path(pathToFile);
    fmt::println("Information about file:");
    auto fExt = file.extension().string();
    auto width = int();
    auto height = int();

    if(fExt == ".bmp"){

    } else { //PNG
        auto buffer = std::vector<std::byte>(8);
        // Musi być ifstream, istream nie działa
        auto input = std::ifstream(file, std::ios_base::binary);
        input.seekg(16); // Pominięcie pierwszych 16 bitów
        input.read(reinterpret_cast<char*>(buffer.data()),8);
        width =   (int(buffer[0]) << 24)
                + (int(buffer[1]) << 16)
                + (int(buffer[2]) << 8)
                + (int(buffer[3]) << 0);
        height =  (int(buffer[4]) << 24)
                + (int(buffer[5]) << 16)
                + (int(buffer[6]) << 8)
                + (int(buffer[7]) << 0);
    }
    fmt::println("- file extension: {}", fExt);
    fmt::println("- size: {} x {}", width, height);

    return 0;
}

auto f_check(std::string const& pathToFile, std::string const& message) -> int{
    fmt::println("CHECK!");
    return 0;
}

auto f_help() -> int{
    fmt::println("HELP!");
    return 0;
}