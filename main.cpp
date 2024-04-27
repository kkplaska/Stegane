#include "fmt/core.h"
#include "functions.hpp"

auto f_info(std::string const& pathToFile) -> int{
    fmt::println("INFO!");
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

auto main(int argc, const char* argv[]) -> int {

    {// Sprawdzenie czy nie ma więcej niż trzy argumenty [flaga, ścieżka do pliku, wiadomość]
        if(argc > 4){
            fmt::println("Too many arguments!");
            return 1;
        }
    }

    enum class Flag {
        INCORRECT, INFO, ENCRYPT, DECRYPT, CHECK, HELP
    };

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

    switch (choseFlag) {
        case Flag::INCORRECT:
            fmt::println("Wrong flag! Try use -h to get help!");
            return 3;
        case Flag::INFO:
        case Flag::DECRYPT:
            if(argc != 3){
                fmt::println("Bad number of arguments!");
                return 2;
            }
            break;
        case Flag::ENCRYPT:
        case Flag::CHECK:
            if(argc != 4){
                fmt::println("Bad number of arguments!");
                return 2;
            }
        case Flag::HELP:
            break;
    }

    auto returnCode = int(0);
    switch (choseFlag) {
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