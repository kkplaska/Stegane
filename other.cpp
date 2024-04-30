#include <filesystem>
#include <fstream>
#include <vector>
#include <chrono>
#include "functions.hpp"
#include "fmt/core.h"
auto f_info(std::string const& pathToFile) -> int{
    auto file = std::filesystem::path(pathToFile);
    fmt::println("Information about file: {}", file.filename().string());
    // File extension
    auto fExt = file.extension().string();
    auto width = int();
    auto height = int();
    { // Size of image
        auto buffer = std::vector<std::byte>(8);
        // Musi być ifstream, istream nie działa
        auto input = std::ifstream(file, std::ios_base::binary);
        if(fExt == ".bmp"){ // BMP
            input.seekg(18); // Pominięcie pierwszych 18 bitów
            input.read(reinterpret_cast<char*>(buffer.data()),8);
            width =   (int(buffer[0]) << 0)
                    + (int(buffer[1]) << 8)
                    + (int(buffer[2]) << 16)
                    + (int(buffer[3]) << 24);
            height =  (int(buffer[4]) << 0)
                    + (int(buffer[5]) << 8)
                    + (int(buffer[6]) << 16)
                    + (int(buffer[7]) << 24);
        } else { //PNG
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
    }
    auto sizeOfFile = std::filesystem::file_size(file);

    // %F = "%Y-%m-%d" | %T = %H:%M:%S
    auto lastWriteTime = std::format("{:%F %T}",std::filesystem::last_write_time(file));

    fmt::println("- file extension: {}", fExt);
    fmt::println("- size of image (in px): {} x {}", width, height);
    fmt::println("- size of file: {} B", sizeOfFile);
    fmt::println("- last write time: {}", lastWriteTime);

    return 0;
}

auto f_help() -> int{
    auto messageToPrint =
            std::string(
            "+----------------------------------+\n"
            "| Project_Steganography.exe - help |\n"
            "+----------------------------------+\n"
            "Project_Steganography.exe [FLAG] [FILEPATH] [MESSAGE]"
            "\n"
            "\n"
            //Zmienić linię poniżej i zastąpić wszystkie lorem ipsumy
            //Opis
            "DESCRIPTION:."
            "\n"
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
            "\n"
            "\n"
            "OPTIONS:"
            "\n"
            "Mandatory arguments to long options are mandatory for short options too.\n"
            "\n"
            "-i, -info [FILEPATH]\n"
            "\tLorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
            "\n"
            "-e, -encrypt [FILEPATH] [MESSAGE]\n"
            "\tLorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
            "\n"
            "-d, -decrypt [FILEPATH]\n"
            "\tLorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
            "\n"
            "-c, -check [FILEPATH] [MESSAGE]\n"
            "\tLorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
            "\n"
            "-h, -help\n"
            "\tLorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
            );
    fmt::println("{}",messageToPrint);
    return 0;
}