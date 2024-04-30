#pragma once
#include <string>
#include <filesystem>
#include <vector>

auto f_info(std::string const& pathToFile) -> int;

auto f_encrypt(std::string const& pathToFile, std::string const& message) -> int;

auto f_decrypt(std::string const& pathToFile) -> int;

auto f_check(std::string const& pathToFile, std::string const& message) -> int;

auto f_help() -> int;

auto getSizeOfImage(std::filesystem::path const& file) -> std::vector<int>;