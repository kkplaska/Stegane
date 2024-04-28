#include "fmt/core.h"
#include "functions.hpp"
#include <filesystem>

auto main(int argc, const char* argv[]) -> int {
    // Sprawdzenie, czy nie ma więcej niż trzy argumenty
    // [flaga, ścieżka do pliku, wiadomość]
    {
        if(argc > 4){
            fmt::println("Too many arguments!");
            return 1;
        }
    }

    enum class Flag {
        INCORRECT, INFO, ENCRYPT, DECRYPT, CHECK, HELP
    };

    // Rozszyfrowanie argumentów
    auto choseFlag = Flag();
    if(argc == 1) choseFlag = Flag::HELP;
    else {
        auto arg = std::string(argv[1]);
        if(arg.size() == 2){
            switch(arg.at(1)){
                case 'i':
                    choseFlag = Flag::INFO;
                    break;
                case 'e':
                    choseFlag = Flag::ENCRYPT;
                    break;
                case 'd':
                    choseFlag = Flag::DECRYPT;
                    break;
                case 'c':
                    choseFlag = Flag::CHECK;
                    break;
                case 'h':
                    choseFlag = Flag::HELP;
                    break;
                default:
                    choseFlag = Flag::INCORRECT;
            }
        }
        else {
            //Dla jednego myślnika at(1), dla dwóch at(2)
            switch(arg.at(1)){
                case 'i':
                    if(arg == "-info"){
                        choseFlag = Flag::INFO;
                    }
                    break;
                case 'e':
                    if(arg == "-encrypt"){
                        choseFlag = Flag::ENCRYPT;
                    }
                    break;
                case 'd':
                    if(arg == "-decrypt"){
                        choseFlag = Flag::DECRYPT;
                    }
                    break;
                case 'c':
                    if(arg == "-check"){
                        choseFlag = Flag::CHECK;
                    }
                    break;
                case 'h':
                    if(arg == "-help"){
                        choseFlag = Flag::HELP;
                    }
                    break;
                default:
                    choseFlag = Flag::INCORRECT;
            }
        }
    }

    // Sprawdzenie, czy są dokładnie dwa argumenty
    // [flaga, ścieżka do pliku]
    if((choseFlag == Flag::INFO || choseFlag == Flag::DECRYPT) && argc != 3){
        fmt::println("Bad number of arguments!");
        return 2;
    }
    // Sprawdzenie, czy są dokładnie trzy argumenty
    // [flaga, ścieżka do pliku, wiadomość]
    if((choseFlag == Flag::CHECK || choseFlag == Flag::ENCRYPT) && argc != 4){
        fmt::println("Bad number of arguments!");
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
            fmt::println("Wrong flag! Try use -h to get help!");
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