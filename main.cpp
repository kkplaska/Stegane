#include "fmt/core.h"
#include "functions.hpp"

auto f_info() -> void {
    fmt::println("INFO!");
}

auto f_check() -> void {
    fmt::println("CHECK!");
}

auto f_help() -> void {
    fmt::println("HELP!");
}

auto main(int argc, const char* argv[]) -> int {
    auto checkTooManyArgs = [argc](int n) -> bool {
        if(argc > n){
            fmt::println("Too many arguments!");
            return true;
        }
        return false;
    };


    // Sprawdzenie czy nie ma więcej niż trzy argumenty [flaga, ścieżka do pliku, wiadomość]
    if(checkTooManyArgs(4)) return 1;

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
        case Flag::HELP:
            f_help();
            break;
        case Flag::INFO:
        case Flag::DECRYPT:
            if(checkTooManyArgs(3)) return 1;

            break;
        case Flag::ENCRYPT:
            f_encrypt();
            break;
        case Flag::CHECK:
            f_check();
    }
    switch (choseFlag) {
        case Flag::INCORRECT:
            break;
        case Flag::INFO:
            f_info();
            break;
        case Flag::ENCRYPT:
            f_encrypt();
            break;
        case Flag::DECRYPT:
            f_decrypt();
            break;
        case Flag::CHECK:
            f_check();
            break;
        case Flag::HELP:
            break;
    }

    return 0;
}