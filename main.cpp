#include "fmt/core.h"
#include "functions.hpp"
#include <filesystem>
#include <map>

auto main(const int argc, const char* argv[]) -> int {
    // Sprawdzenie, czy nie ma więcej niż trzy argumenty
    // [flaga, ścieżka do pliku, wiadomość]
    {
        if(argc > 4){
            fmt::println("The program has been launched with too many arguments!");
            return 1;
        }
    }

    enum class Flag {
        INCORRECT, INFO, ENCRYPT, DECRYPT, CHECK, HELP
    };

    // Rozszyfrowanie argumentów
    Flag choseFlag;
    if(argc == 1) choseFlag = Flag::HELP;
    else {
        auto flags = std::map<std::string, Flag>{
                {"-i",Flag::INFO},
                {"-info",Flag::INFO},
                {"-e",Flag::ENCRYPT},
                {"-encrypt",Flag::ENCRYPT},
                {"-d",Flag::DECRYPT},
                {"-decrypt",Flag::DECRYPT},
                {"-c",Flag::CHECK},
                {"-check",Flag::CHECK},
                {"-h",Flag::HELP},
                {"-help",Flag::HELP}
        };
        // Sprawdza, czy flaga znajduje się w mapie.
        // Jeżeli nie, dodaje ją i traktuje jako niepoprawną
        flags.try_emplace(argv[1],Flag::INCORRECT);
        choseFlag = flags[argv[1]];
    }

    // Sprawdzenie, czy są dokładnie dwa argumenty
    // [flaga, ścieżka do pliku]
    if((choseFlag == Flag::INFO || choseFlag == Flag::DECRYPT) && argc != 3){
        fmt::println("The program has been launched with bad number of arguments!");
        return 2;
    }
    // Sprawdzenie, czy są dokładnie trzy argumenty
    // [flaga, ścieżka do pliku, wiadomość]
    if((choseFlag == Flag::CHECK || choseFlag == Flag::ENCRYPT) && argc != 4){
        fmt::println("The program has been launched with bad number of arguments!");
        return 2;
    }

    // Sprawdzenie, czy plik jest obsługiwany
    if(argc > 1 && choseFlag != Flag::INCORRECT && choseFlag != Flag::HELP){
        auto file = std::filesystem::path(argv[2]);
        if(!std::filesystem::exists(file)){
            fmt::println("The path is wrong, or the file doesn't exist!");
            return 3;
        }
        if(!std::filesystem::is_regular_file(file)){
            fmt::println("The file is corrupted!");
            return 4;
        }
        if(file.extension() != ".bmp" && file.extension() != ".png"){
            fmt::println("{} is wrong format of the file! \n Try using a program with .bmp or .png file", file.extension().string());
            return 5;
        }
        // Sprawdzenie uprawnień do odczytu
        // Ewentualnie do dalszego rozważenia
        if(
            std::filesystem::perms::none == (std::filesystem::status(file).permissions() & std::filesystem::perms::owner_read)
            || std::filesystem::perms::none == (std::filesystem::status(file).permissions() & std::filesystem::perms::owner_write)
         ){
            fmt::println("Access to the file {} is not allowed!", file.filename().string());
            return 6;
        }

    }

    // Wskazania dalszego działania programu w zgodzie z flagą
    int returnCode;
    switch (choseFlag) {
        case Flag::INCORRECT:
            fmt::println("The program has been launched with wrong flag! Try use -h to get help!");
            returnCode = 7;
            break;
        case Flag::INFO:
            returnCode = f_info(std::string(argv[2]));
            break;
        case Flag::ENCRYPT:
            returnCode = f_encrypt(std::string(argv[2]),std::string(argv[3]));
            break;
        case Flag::DECRYPT:
            returnCode = f_decrypt(std::string(argv[2]));
            break;
        case Flag::CHECK:
            returnCode = f_check(std::string(argv[2]),std::string(argv[3]));
            break;
        case Flag::HELP:
            returnCode = f_help();
            break;
    }

    return returnCode;
}