#pragma once
#include <string>
#include <filesystem>
#include <vector>

struct SizeOfImage {
    const int width;
    const int height;
    public: SizeOfImage(int const& w, int const& h) : width(w), height(h) { };
};

auto f_info(std::filesystem::path const& file) -> int;

auto f_encrypt(std::filesystem::path const& file, std::string const& message) -> int;

auto f_decrypt(std::filesystem::path const& file) -> int;

auto f_check(std::filesystem::path const& file, std::string const& message) -> int;

auto f_help() -> int;

namespace BMP {
    auto getSizeOfImage(std::filesystem::path const& file) -> SizeOfImage;
    auto getNumBPP(std::filesystem::path const& file) -> int;
    auto getNumOfPadding(SizeOfImage const& soi) -> int;
}

namespace PNG {
    auto getSizeOfImage(std::filesystem::path const& file) -> SizeOfImage;
}

