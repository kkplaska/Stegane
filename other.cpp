#include "functions.hpp"

auto getFileFormat(std::filesystem::path const& file) -> FileFormat{
    auto ext = file.extension().string();
    if(ext == ".bmp"){
        return FileFormat::BMP;
    } else {
        return FileFormat::SFML_Format;
    }
}
auto getMaxMessageSize(SizeOfImage const& sizeOfImage) -> int {
    auto numberOfBytes = sizeOfImage.width * sizeOfImage.height / 8;
    numberOfBytes *= 3;
    // 4 pierwsze bajty zajęte przez długość wiadomości
    numberOfBytes -= 4;
    return numberOfBytes;
}

namespace PNG {
    auto getSizeOfImage(std::filesystem::path const& file) -> SizeOfImage{
        auto size = std::vector<int>(2);
        auto buffer = std::vector<char>(8);
        auto input = std::fstream(file, std::ios::binary | std::ios::in);
        input.seekg(0x10); // Skok do 16 bitu
        // https://en.cppreference.com/w/cpp/io/basic_istream/seekg
        input.read(&buffer[0],8);
        //Width
        auto width = int((int(buffer[0]) << 24)
                       | (int(buffer[1]) << 16)
                       | (int(buffer[2]) << 8)
                       | (int(buffer[3]) << 0));
        //Height
        auto height = int((int(buffer[4]) << 24)
                       | (int(buffer[5]) << 16)
                       | (int(buffer[6]) << 8)
                       | (int(buffer[7]) << 0));
        return SizeOfImage{width, height};
    }
}

namespace BMP{
    auto getSizeOfImage(std::filesystem::path const& file) -> SizeOfImage{
        auto size = std::vector<int>(2);
        auto buffer = std::vector<char>(8);
        auto input = std::fstream(file, std::ios::binary | std::ios::in);
        input.seekg(0x12); // Skok do 18 bitu
        // https://en.cppreference.com/w/cpp/io/basic_istream/seekg
        input.read(&buffer[0],8);
        //Width
        auto width = int((int(buffer[0]) << 0)
                       | (int(buffer[1]) << 8)
                       | (int(buffer[2]) << 16)
                       | (int(buffer[3]) << 24));
        //Height
        auto height = int((int(buffer[4]) << 0)
                        | (int(buffer[5]) << 8)
                        | (int(buffer[6]) << 16)
                        | (int(buffer[7]) << 24));
        return SizeOfImage{width, height};
    }

    auto getNumOfPadding(SizeOfImage const& soi) -> int{
        return soi.width % 4;
    }
}

namespace sfmlImg {
    auto getSizeOfImage(std::filesystem::path const& file) -> SizeOfImage{
        auto img = sf::Image();
        img.loadFromFile(file.string());
        int width = int(img.getSize().x);
        int height = int(img.getSize().y);
        return SizeOfImage{width, height};
    }
}

auto sizeOfImageHelper(std::filesystem::path const& file) -> SizeOfImage{
    if(file.extension() == ".bmp") {
        return BMP::getSizeOfImage(file);
    }
    else if (file.extension() == ".png") {
        return PNG::getSizeOfImage(file);
    }
    else {
        return sfmlImg::getSizeOfImage(file);
    }
}
auto checkMessageSize(int size, SizeOfImage const& sizeOfImage) -> void{
    if(size > getMaxMessageSize(sizeOfImage)) throw std::logic_error("The file has got corrupted message or the file has not got a message!");
}

auto tempFile (std::filesystem::path const& inputFile) -> std::filesystem::path {
    auto outputFile = std::filesystem::path(inputFile.parent_path());
    outputFile /= "temp";
    outputFile += inputFile.extension();
    return outputFile;
}