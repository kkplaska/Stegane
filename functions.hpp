#pragma once
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <exception>
#include <stdexcept>
#include "fmt/core.h"
#include "SFML/Graphics/Image.hpp"
#include <SFML/Graphics/Color.hpp>

struct SizeOfImage {
    const int width;
    const int height;
    public: SizeOfImage(int const& w, int const& h) : width(w), height(h) { };
};

enum class FileFormat {
    BMP, SFML_Format
};

auto f_info(std::filesystem::path const& file) -> void;

auto f_encrypt(std::filesystem::path const& file, std::string message) -> void;

auto f_decrypt(std::filesystem::path const& file) -> void;

auto f_check(std::filesystem::path const& file, std::string const& message) -> void;

auto f_help() -> void;

namespace BMP {
    auto getSizeOfImage(std::filesystem::path const& file) -> SizeOfImage;
    auto getNumOfPadding(SizeOfImage const& soi) -> int;
}

namespace PNG {
    auto getSizeOfImage(std::filesystem::path const& file) -> SizeOfImage;
}

namespace sfmlImg {
    auto getSizeOfImage(std::filesystem::path const& file) -> SizeOfImage;
}

auto getMaxMessageSize(SizeOfImage const& sizeOfImage) -> int;
auto getFileFormat(std::filesystem::path const& file) -> FileFormat;
auto sizeOfImageHelper(std::filesystem::path const& file) -> SizeOfImage;
auto checkMessageSize(int size, SizeOfImage const& sizeOfImage) -> void;
auto tempFile (std::filesystem::path const& inputFile) -> std::filesystem::path;