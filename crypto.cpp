#include "functions.hpp"
#include "fmt/ranges.h"

namespace BMP {
    auto encrypt(std::filesystem::path const& file,
                 SizeOfImage const& sizeOfImage,
                 std::string const& message
    ) -> int {
        auto outputFile = std::filesystem::path(file.parent_path());
        outputFile /= "temp";
        outputFile += file.extension();
        auto output = std::fstream(outputFile, std::ios::binary | std::ios::out | std::ios::trunc);

        // DEBUG
        fmt::println("{}", outputFile.string());

        auto input = std::fstream(file, std::ios::binary | std::ios::in);


        auto buffer = std::vector<char>(512);
        char startPosition;
        input.seekg(0xA);
        input.read(&startPosition,1);
        input.seekg(0);

        // Przepisywanie nagłówka
        {
            auto miniBuffer = std::vector<char>(startPosition);
            input.read(&miniBuffer[0], startPosition);
            output.write(&miniBuffer[0], startPosition);
        }

        {
            input.read(&buffer[0],ssize_t(buffer.size()));

            // Dla plików mniejszych niż 512 B
            if(buffer.size() != input.gcount()){
                auto tmp = std::vector<char>(buffer.begin(), buffer.begin() + input.gcount());
                buffer = tmp;
            }

            auto bufferIndex = int(0);
            auto lineIndex = int(0);

            for (auto character : message) {

                for (int i = 0; i < 8; ++i) {
                    // 0x80 == 0b1000_0000
                    if((character & 0x80) == 0x80){
                        buffer.at(bufferIndex) = char(buffer.at(bufferIndex) | 1);
                    } else {
                        //0xFE == 0b1111_1110
                        buffer.at(bufferIndex) = char(buffer.at(bufferIndex) & 0xFE);
                    }
                    character <<= 1;
                    bufferIndex++;
                    lineIndex++;

                    // Pilnowanie paddingów
                    if(lineIndex == sizeOfImage.width){
                        lineIndex = 0;
                        for (int j = 0; j < sizeOfImage.width % 4; ++j) {
                            bufferIndex++;
                        }
                    }
                }

                if(bufferIndex == buffer.size()){
                    output.write(&buffer[0],ssize_t(buffer.size()));
                    bufferIndex = 0;
                    input.read(&buffer[0],ssize_t(buffer.size()));
                }

            }
            output.write(&buffer[0],input.gcount());

            if(buffer.size() != input.gcount()){
                output.write(&buffer[0],input.gcount());
            }
            else {
                while(input.read(&buffer[0],ssize_t(buffer.size()))){
                    output.write(&buffer[0],input.gcount());
                }

                if(buffer.size() != input.gcount()){
                    output.write(&buffer[0],input.gcount());
                }
            }
        }
        return 0;
    }
}

namespace sfmlImg {
    auto encrypt(std::filesystem::path const& file,
                  SizeOfImage const& sizeOfImage,
                  std::string const& message
    ) -> int {
        auto bitMessage = std::vector<char>();
        for (auto character: message) {
            for (int i = 0; i < 8; ++i) {
                // 0x80 == 0b1000_0000
                if((character & 0x80) == 0x80){
                    bitMessage.push_back(1);
                } else {
                    bitMessage.push_back(0);
                }
                character <<= 1;
            }
        }

        auto setColor = [](uint8_t& color, char& bit) -> void {
            // 0x80 == 0b1000_0000
            if(bit == 1){
                color |= 1;
            } else {
                //0xFE == 0b1111_1110
                color &= 0xFFFE;
            }
        };

        auto imgFile = sf::Image();
        imgFile.loadFromFile(file.string());
        auto x = int(0);
        auto y = int(0);
        for (int i = 0; i < bitMessage.size(); ) {
            auto pixel = imgFile.getPixel(x, y);
            if(i + 1 > bitMessage.size()) break;
            setColor(pixel.r, bitMessage.at(i));

            if(i + 2 > bitMessage.size()) break;
            setColor(pixel.g, bitMessage.at(i + 1));

            if(i + 3 > bitMessage.size()) break;
            setColor(pixel.b, bitMessage.at(i + 2));
            ++x;
            if(x == sizeOfImage.width + 1){
                x = 0;
                ++y;
            }
            i += 3;
        }

        auto str = std::string("..\\testVectors\\temp" + file.extension().string());
        fmt::println("{}", str);
        imgFile.saveToFile(str);

        return 0;
    }
}

auto prepareMessage(std::string& message) -> void {

    //Pierwsze 4 bajty -> długość wiadomości
    auto sizeOfMessage = int(message.size());
    auto firstNumber = char(53);

    // 4, bo 4 Bajty
    for (int i = 0; i < 4; ++i) {
        message.insert(i, 1, char(firstNumber + ((sizeOfMessage >> (32 - 8 * (i + 1))) & 255)));
    }

    //XOR-owanie wiadomości
    for (auto& character : message) {
        character ^= firstNumber;
    }

}

auto f_encrypt(std::filesystem::path const& file, std::string message) -> int {
    auto extension = getFileFormat(file);
    auto sizeOfImage = sizeOfImageHelper(file);
    if(message.size() > getMaxMessageSize(sizeOfImage)){
        fmt::println("A message can not be encrypt in file {}!", file.filename().string());
        return 8;
    }

    prepareMessage(message);

    // DEBUG
//    {
//        for (auto c : message) {
//            fmt::print("{} ", int(c));
//        }
//        fmt::println("");
//        for (auto c : message) {
//            fmt::print("{0:b} ", int(c));
//        }
//        fmt::println("");
//    }

    auto returnCode = int(0);
    switch(extension){
        case FileFormat::BMP:
            returnCode = BMP::encrypt(file, sizeOfImage, message);
            break;
        case FileFormat::PNG:
        case FileFormat::SFML_Format:
            returnCode = sfmlImg::encrypt(file, sizeOfImage, message);
            break;
    }

    fmt::println("ENCRYPT!");
    return returnCode;

    //Usunięcie pierwotnego pliku
    //Zmiana nazwy temp na pierwotną
}

auto f_check(std::filesystem::path const& file, std::string const& message) -> int{
    auto numberOfChar = message.size();
    auto sizeOfImage = sizeOfImageHelper(file);
    if(numberOfChar <= getMaxMessageSize(sizeOfImage)){
        fmt::println("A message can be encrypt in file {}!", file.filename().string());
        return 0;
    }
    else {
        fmt::println("A message can not be encrypt in file {}!", file.filename().string());
        return 8;
    }

}