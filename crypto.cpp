#include "functions.hpp"
#include "fmt/core.h"

auto f_encrypt(std::string const& pathToFile, std::string const& message) -> int{
    fmt::println("ENCRYPT!");
    return 12;
}

auto f_decrypt(std::string const& pathToFile) -> int{
    fmt::println("DECRYPT!");
}