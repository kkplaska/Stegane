#pragma once
#include <string>

auto f_info(std::string const& pathToFile) -> int;

auto f_encrypt(std::string const& pathToFile, std::string const& message) -> int;

auto f_decrypt(std::string const& pathToFile) -> int;

auto f_check(std::string const& pathToFile, std::string const& message) -> int;

auto f_help() -> int;