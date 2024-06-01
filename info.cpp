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
            "DESCRIPTION:."
            "\n"
            "\tHide message in graphic file. Read hidden message from prepared graphic file."
            "\n"
            "Available formats of graphic files:"
            "\n"
            "\t .bmp | .png | .tga\n"
            "\n"
            "Fields:"
            "\n"
            "\t[FLAG] - options from below, which allow manipulating  the running of the program."
            "\n"
            "\t[FILEPATH] - a path to graphic file."
            "\n"
            "\t[MESSAGE] - a message to hide, which contain only ASCII characters. Use of other characters is forbidden!"
            "\n"
            "\n"
            "OPTIONS:"
            "\n"
            "[Mandatory arguments to long options are mandatory for short options too!]\n"
            "\n"
            "-i, --info [FILEPATH]\n"
            "\tShow information about the file like: extension, size of graphic, size of file and last edit timestamp.\n"
            "\n"
            "-e, --encrypt [FILEPATH] [MESSAGE]\n"
            "\tHide the message in the file.\n"
            "\n"
            "-d, --decrypt [FILEPATH]\n"
            "\tRead hidden message from the file.\n"
            "\n"
            "-c, --check [FILEPATH] [MESSAGE]\n"
            "\tChecking if is possible to hide a message inside the file.\n"
            "\n"
            "-h, --help\n"
            "\tDisplay this help.\n"
            );
    fmt::println("{}",messageToPrint);
}

