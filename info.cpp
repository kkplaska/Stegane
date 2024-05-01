#include <fstream>
#include <chrono>
#include "functions.hpp"
#include "fmt/core.h"
auto f_info(std::string const& pathToFile) -> int{
    auto file = std::filesystem::path(pathToFile);
    fmt::println("Information about file: {}", file.filename().string());
    // File extension
    auto fExt = file.extension().string();
    auto sizeOfImageLambda = [&]()->SizeOfImage{
        if(fExt == ".bmp") {
            return BMP::getSizeOfImage(file);
        }
        else if (fExt == ".png") {
            return PNG::getSizeOfImage(file);
        }
        return SizeOfImage{0,0};
    };
    auto sizeOfImage = sizeOfImageLambda();
    auto sizeOfFile = std::filesystem::file_size(file);
    // %F = "%Y-%m-%d" | %T = %H:%M:%S
    auto lastWriteTime = std::format("{:%F %T}",std::filesystem::last_write_time(file));

    fmt::println("- file extension: {}", fExt);
    fmt::println("- size of image (in px): {} x {}", sizeOfImage.width, sizeOfImage.height);
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

