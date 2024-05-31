#include "functions.hpp"
#include <map>

auto main(const int argc, const char* argv[]) -> int {
    // Sprawdzenie, czy nie ma więcej niż trzy argumenty
    // [flaga, ścieżka do pliku, wiadomość]
    if (argc > 4) {
        fmt::println("The program has been launched with too many arguments!");
        return 2;
    }

    enum class Flag {
        INCORRECT, INFO, ENCRYPT, DECRYPT, CHECK, HELP
    };

    // Rozszyfrowanie argumentów
    Flag choseFlag;
    if (argc == 1) choseFlag = Flag::HELP;
    else {
        auto flags = std::map<std::string, Flag>{
                {"-i",        Flag::INFO},
                {"--info",    Flag::INFO},
                {"-e",        Flag::ENCRYPT},
                {"--encrypt", Flag::ENCRYPT},
                {"-d",        Flag::DECRYPT},
                {"--decrypt", Flag::DECRYPT},
                {"-c",        Flag::CHECK},
                {"--check",   Flag::CHECK},
                {"-h",        Flag::HELP},
                {"--help",    Flag::HELP}
        };
        // Sprawdza, czy flaga znajduje się w mapie.
        // Jeżeli nie, dodaje ją i traktuje jako niepoprawną
        // https://en.cppreference.com/w/cpp/container/map/try_emplace
        flags.try_emplace(argv[1], Flag::INCORRECT);
        choseFlag = flags[argv[1]];
    }

    // Lambdy
    auto checkNumOfArguments = [&argc](int n) -> void {
        ++n;
        if (argc == n){
            return;
        } else if (argc < n) {
            throw std::logic_error("The program has been launched with too few arguments!");
        } else {
            throw std::logic_error("The program has been launched with too many arguments!");
        }
    };
    auto checkFile = [&argv]() -> void {
        auto file = std::filesystem::path(argv[2]);
        if (!std::filesystem::exists(file)) {
            throw std::logic_error("Stated path is wrong, or the file doesn't exist!");
        }
        if (!std::filesystem::is_regular_file(file)) {
            throw std::logic_error(fmt::format("The file {} is corrupted!", file.filename().string()));
        }
        if (file.extension() != ".bmp"
        && file.extension() != ".png"
        && file.extension() != ".tga"
        ) {
            throw std::logic_error(fmt::format("{} is wrong format of the file! \n Try using a program with .bmp or .png file", file.extension().string()));
        }
        // Sprawdzenie uprawnień do odczytu
        auto fs = std::fstream(file);
        if (!fs.is_open()){
            throw std::logic_error(fmt::format("Access to the file {} is not allowed!", file.filename().string()));
        }
    };

    try{switch (choseFlag) {
        case Flag::INCORRECT:
            break;
        case Flag::HELP:
            checkNumOfArguments(1);
            break;
        case Flag::INFO:    // Sprawdzenie, czy są dokładnie dwa argumenty
        case Flag::DECRYPT: // [flaga, ścieżka do pliku]
            checkNumOfArguments(2);
            break;
        case Flag::ENCRYPT: // Sprawdzenie, czy są dokładnie trzy argumenty
        case Flag::CHECK:   // [flaga, ścieżka do pliku, wiadomość]
            checkNumOfArguments(3);
            break;
    }} catch (std::exception const& e){
        fmt::println("{}", e.what());
        return 2;
    }
    
    try{switch (choseFlag) {
        case Flag::INFO:
        case Flag::DECRYPT:
        case Flag::ENCRYPT:
        case Flag::CHECK:
            checkFile();
        case Flag::INCORRECT:
        case Flag::HELP:
            break;
    }} catch (std::exception const& e){
        fmt::println("{}", e.what());
        return 3;
    }

    // Wskazania dalszego działania programu w zgodzie z flagą
    try{switch (choseFlag) {
        case Flag::INCORRECT:
            fmt::println("The program has been launched with wrong flag! Try use -h to get help!");
            return 4;
        case Flag::INFO:
            f_info(std::filesystem::path(argv[2]));
            break;
        case Flag::ENCRYPT:
            f_encrypt(std::filesystem::path(argv[2]),std::string(argv[3]));
            break;
        case Flag::DECRYPT:
            f_decrypt(std::filesystem::path(argv[2]));
            break;
        case Flag::CHECK:
            f_check(std::filesystem::path(argv[2]),std::string(argv[3]));
            break;
        case Flag::HELP:
            f_help();
            break;
    }} catch (std::exception const& e){
        fmt::println("{}", e.what());
        return 5;
    }
    return 0;
}