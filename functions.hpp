#pragma once
#include <string>

auto f_info(std::string pathToFile) -> void;

auto f_encrypt(std::string pathToFile, std::string message) -> void;

auto f_decrypt(std::string pathToFile) -> void;

auto f_check(std::string pathToFile, std::string message) -> void;

auto f_help() -> void;