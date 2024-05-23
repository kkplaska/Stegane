#include "functions.hpp"
#include <map>

auto main(const int argc, const char* argv[]) -> int {
    // Sprawdzenie, czy nie ma więcej niż trzy argumenty
    // [flaga, ścieżka do pliku, wiadomość]
    {
        if (argc > 4) {
            fmt::println("The program has been launched with too many arguments!");
            return 1;
        }
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
    auto checkNumOfArguments = [argc](int n) -> int {
        ++n;
        if (argc != n) {
            fmt::println("The program has been launched with bad number of arguments!");
            return 2;
        }
        return 0;
    };
    auto checkFile = [argv]() -> int {
        auto file = std::filesystem::path(argv[2]);
        if (!std::filesystem::exists(file)) {
            fmt::println("The path is wrong, or the file doesn't exist!");
            return 3;
        }
        if (!std::filesystem::is_regular_file(file)) {
            fmt::println("The file is corrupted!");
            return 4;
        }
        if (file.extension() != ".bmp" && file.extension() != ".ppm") {
            fmt::println("{} is wrong format of the file! \n Try using a program with .bmp or .ppm file",
                         file.extension().string());
            return 5;
        }
        // Sprawdzenie uprawnień do odczytu
        auto fs = std::fstream(file);
        if (!fs.is_open()){
            fmt::println("Access to the file {} is not allowed!", file.filename().string());
            return 6;
        }
        fs.close();

        // Jeżeli wszystko w porządku zwracamy 0
        return 0;
    };

    int returnCode;
    switch (choseFlag) {
        case Flag::INCORRECT:
            break;
        case Flag::HELP:
            returnCode = checkNumOfArguments(1);
            if(returnCode == 0) break;
            return returnCode;
        case Flag::INFO:
        case Flag::DECRYPT:
            // Sprawdzenie, czy są dokładnie dwa argumenty
            // [flaga, ścieżka do pliku]
            returnCode = checkNumOfArguments(2);
            if(returnCode == 0){
                returnCode = checkFile();
                if(returnCode == 0) break;
            }
            return returnCode;
        case Flag::ENCRYPT:
        case Flag::CHECK:
            // Sprawdzenie, czy są dokładnie trzy argumenty
            // [flaga, ścieżka do pliku, wiadomość]
            returnCode = checkNumOfArguments(3);
            if(returnCode == 0){
                returnCode = checkFile();
                if(returnCode == 0) break;
            }
            return returnCode;
    }

    // Wskazania dalszego działania programu w zgodzie z flagą
    switch (choseFlag) {
        case Flag::INCORRECT:
            fmt::println("The program has been launched with wrong flag! Try use -h to get help!");
            returnCode = 7;
            break;
        case Flag::INFO:
            returnCode = f_info(std::filesystem::path(argv[2]));
            break;
        case Flag::ENCRYPT:
            returnCode = f_encrypt(std::filesystem::path(argv[2]),std::string(argv[3]));
            break;
        case Flag::DECRYPT:
            returnCode = f_decrypt(std::filesystem::path(argv[2]));
            break;
        case Flag::CHECK:
            returnCode = f_check(std::filesystem::path(argv[2]),std::string(argv[3]));
            break;
        case Flag::HELP:
            returnCode = f_help();
            break;
    }

    return returnCode;
}