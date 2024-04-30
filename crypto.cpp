#include "fmt/core.h"
#include "functions.hpp"

auto f_encrypt(std::string const& pathToFile, std::string const& message) -> int{
    fmt::println("ENCRYPT!");
    return 12;
}

auto f_decrypt(std::string const& pathToFile) -> int{
    fmt::println("DECRYPT!");
    return 15;
}

auto f_check(std::string const& pathToFile, std::string const& message) -> int{
    fmt::println("CHECK!");
    return 0;
}