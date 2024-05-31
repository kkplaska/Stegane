#include <fstream>
#include "functions.hpp"
#include "fmt/core.h"
#include "fmt/chrono.h"
auto f_info(std::filesystem::path const& file) -> void {
    fmt::println("Information about file: {}", file.filename().string());
    // File extension
    auto fExt = file.extension().string();
    auto sizeOfImage = sizeOfImageHelper(file);
    // https://en.cppreference.com/w/cpp/filesystem/file_size
    auto sizeOfFile = std::filesystem::file_size(file);
    // %F = "%Y-%m-%d" | %T = %H:%M:%S
    auto lastWriteTime = fmt::format("{:%F %T}",
         std::chrono::time_point_cast<std::chrono::seconds>( // Zaokrąglenie do sekund
         std::chrono::file_clock::to_sys(
             std::filesystem::last_write_time(file))));
    // Formatowanie fmt - https://fmt.dev/latest/syntax.html#chrono-specs
    // Alternatywa dla niedziałającego na uczelni std::format - https://stackoverflow.com/questions/61030383/how-to-convert-stdfilesystemfile-time-type-to-time-t
    // Zaokrąglenie do sekund - https://stackoverflow.com/questions/76106361/stdformating-stdchrono-seconds-without-fractional-digits

    fmt::println("- file extension: {}", fExt);
    fmt::println("- size of image (in px): {} x {}", sizeOfImage.width, sizeOfImage.height);
    fmt::println("- size of file: {} B", sizeOfFile);
    fmt::println("- last write time: {}", lastWriteTime);
}

auto f_help() -> void {
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
            "-i, --info [FILEPATH]\n"
            "\tLorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
            "\n"
            "-e, --encrypt [FILEPATH] [MESSAGE]\n"
            "\tLorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
            "\n"
            "-d, --decrypt [FILEPATH]\n"
            "\tLorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
            "\n"
            "-c, --check [FILEPATH] [MESSAGE]\n"
            "\tLorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
            "\n"
            "-h, --help\n"
            "\tLorem ipsum dolor sit amet, consectetur adipiscing elit.\n"
            );
    fmt::println("{}",messageToPrint);
}

