#include "functions.hpp"
#include "fmt/ranges.h"

namespace BMP {
auto encrypt(std::filesystem::path const& file,
             SizeOfImage const& sizeOfImage,
             std::string const& message
) -> void {
    auto output = std::fstream(tempFile(file), std::ios::binary | std::ios::out | std::ios::trunc);
    auto input = std::fstream(file, std::ios::binary | std::ios::in);

    auto buffer = std::vector<char>(512);
    char startPosition;
    // https://en.cppreference.com/w/cpp/io/basic_istream/seekg
    input.seekg(0xA);
    input.read(&startPosition,1); //Odczytywanie pozycji końca nagłówka
    input.seekg(0);

    // Przepisywanie nagłówka -> tutaj bufor ma rozmiar nagłówka
    auto miniBuffer = std::vector<char>(startPosition);
    input.read(&miniBuffer[0], startPosition);
    output.write(&miniBuffer[0], startPosition);

    input.read(&buffer[0],static_cast<std::streamsize>(buffer.size()));

    // Dla plików mniejszych niż rozmiar bufora
    if(buffer.size() != input.gcount()){
        auto tmp = std::vector<char>(buffer.begin(), buffer.begin() + input.gcount());
        buffer = tmp;
    }

    auto bufferIndex = int(0);
    auto lineIndex = int(0);

    for (auto character : message) {
        for (int i = 0; i < 8; ++i) {
            if((character & 0x80) == 0x80){ //0x80 == 0b1000_0000
                buffer.at(bufferIndex) |=  1;
            } else { //0xFE == 0b1111_1110
                buffer.at(bufferIndex) &= -2; // Signed -2 = Unsigned 254 0xFE
            }

            character <<= 1;
            bufferIndex++;
            lineIndex++;

            if(lineIndex == sizeOfImage.width){ // Pilnowanie paddingów
                lineIndex = 0;
                for (int j = 0; j < getNumOfPadding(sizeOfImage); ++j) {
                    bufferIndex++;
                }
            }
        }

        if(bufferIndex == buffer.size()){
            output.write(&buffer[0],static_cast<std::streamsize>(buffer.size()));
            bufferIndex = 0;
            input.read(&buffer[0],static_cast<std::streamsize>(buffer.size()));
        }

    }
    output.write(&buffer[0],input.gcount());

    if(buffer.size() != input.gcount()){
        output.write(&buffer[0],input.gcount());
    }
    else {
        while(input.read(&buffer[0],static_cast<std::streamsize>(buffer.size()))){
            output.write(&buffer[0],input.gcount());
        }

        if(buffer.size() != input.gcount()){
            output.write(&buffer[0],input.gcount());
        }
    }
}}

namespace sfmlImg {
auto encrypt(std::filesystem::path const& file,
              SizeOfImage const& sizeOfImage,
              std::string const& message
) -> void {
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

    auto biasColor = [](uint8_t& color, char& bit) -> void {
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

        if(i == bitMessage.size()) break;
        biasColor(pixel.r, bitMessage.at(i));
        if(i + 1 == bitMessage.size()) break;
        biasColor(pixel.g, bitMessage.at(i + 1));
        if(i + 2 == bitMessage.size()) break;
        biasColor(pixel.b, bitMessage.at(i + 2));

        imgFile.setPixel(x, y, pixel);
        ++x;
        if(x == sizeOfImage.width){
            x = 0;
            ++y;
        }
        i += 3;
    }

    imgFile.saveToFile(tempFile(file).string());
}}

auto prepareMessage(std::string& message) -> void {
    auto sizeOfMessage = int(message.size());
    auto firstNumber = char(53);
    //Pierwsze 4 bajty -> długość wiadomości
    for (int i = 0; i < 4; ++i) {
        message.insert(i, 1, char(firstNumber + ((sizeOfMessage >> (32 - 8 * (i + 1))) & 255)));
    }
    //XOR-owanie wiadomości
    for (auto& character: message) character ^= 53;
}

auto f_encrypt(std::filesystem::path const& file, std::string message) -> void {
    auto extension = getFileFormat(file);
    auto sizeOfImage = sizeOfImageHelper(file);
    if(message.size() > getMaxMessageSize(sizeOfImage)){
        throw std::logic_error(fmt::format("A message can not be encrypt in file {}!", file.filename().string()));
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

    switch(extension){
        case FileFormat::BMP:
            BMP::encrypt(file, sizeOfImage, message);
            break;
        case FileFormat::SFML_Format:
            sfmlImg::encrypt(file, sizeOfImage, message);
            break;
    }

    //Usunięcie pierwotnego pliku i Zmiana nazwy temp na pierwotną
    auto outputFile = tempFile(file);
    //https://en.cppreference.com/w/cpp/filesystem/remove
    std::filesystem::remove(file);
    //https://en.cppreference.com/w/cpp/filesystem/rename
    std::filesystem::rename(outputFile, file);

    fmt::println("The message has been encrypted successfully!");
}

auto f_check(std::filesystem::path const& file, std::string const& message) -> void {
    auto numberOfChar = message.size();
    auto sizeOfImage = sizeOfImageHelper(file);
    if(numberOfChar <= getMaxMessageSize(sizeOfImage)){
        fmt::println("A message can be encrypt in file {}!", file.filename().string());
    } else {
        throw std::logic_error(fmt::format("The message can not be encrypt in file {}!", file.filename().string()));
    }
}