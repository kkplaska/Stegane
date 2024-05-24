#include "functions.hpp"

namespace BMP {
    auto decrypt(std::filesystem::path const& file, SizeOfImage const& sizeOfImage) -> int {
        auto input = std::fstream(file, std::ios::binary | std::ios::in);
        auto bufferSize = int(512);
        auto buffer = std::vector<char>(bufferSize);

        char startPosition;
        input.seekg(0xA);
        input.read(&startPosition,1);
        input.seekg(startPosition);

        auto sizeOfMessage = int();
        auto lineIndex = int(0);
        // Number Of Paddings
        auto nop = BMP::getNumOfPadding(sizeOfImage);

        // Pierwszy bufor 512B
        auto readBufferIndex = int(0);
        input.read(&buffer[0], bufferSize);

        auto paddingHelper = [&]() mutable -> void {
            if(lineIndex == sizeOfImage.width){
                lineIndex = 0;
                for (int j = 0; j < nop; ++j) {
                    readBufferIndex++;
                }
            }
        };

        auto bufferANDHelper = [&]() mutable -> void {
            for (int i = 0; i < input.gcount(); ++i) {
                buffer.at(i) &= 1;
            }
        };

        bufferANDHelper();

        // Pierwsze 4 Bajty
        {
            // som <> SizeOfMessage
            auto somBuffer = std::vector<char>(4);
            for(char& somElement : somBuffer) {
                for (int j = 0; j < 8; ++j, ++readBufferIndex) {
                    somElement <<= 1;
                    somElement |= buffer.at(readBufferIndex);
                }
                somElement ^= 53;
                somElement -= 53;
                sizeOfMessage <<= 8;
                sizeOfMessage += somElement;
                lineIndex++;

                // Pilnowanie paddingów
                paddingHelper();
            }
        }

        auto messageBuffer = std::vector<char>(sizeOfMessage);
        auto messageBufferIndex = int(0);
        auto byteVisor = int(0);

        // Kontynuacja do 512 B lub końca, jeśli jest mniejszy
        auto nextBuffer = [&]() mutable -> bool {
            for ( ; readBufferIndex < input.gcount(); ++readBufferIndex){
                messageBuffer.at(messageBufferIndex) <<= 1;
                messageBuffer.at(messageBufferIndex) |= buffer.at(readBufferIndex);

                byteVisor++;
                if(byteVisor % 8 == 0){
                    byteVisor = 0;
                    messageBufferIndex++;
                }

                if(messageBufferIndex == sizeOfMessage){
                    return true;
                }

                // Pilnowanie paddingów
                paddingHelper();
            }
            return false;
        };
        nextBuffer();

        while(messageBufferIndex < sizeOfMessage){
            input.read(&buffer[0], bufferSize);
            readBufferIndex = int(0);
            bufferANDHelper();
            if(nextBuffer()){
                break;
            }
        }

        for(auto& character : messageBuffer){
            character ^= 53;
        }

        auto message = std::string(messageBuffer.begin(),messageBuffer.end());
        fmt::println("{}",message);
        return 0;
    }
}

namespace sfmlImg {
    auto decrypt(std::filesystem::path const& file, SizeOfImage const& sizeOfImage) -> int {
        auto imgFile = sf::Image();
        imgFile.loadFromFile(file.string());
        auto x = int(0);
        auto y = int(0);

        auto getColor = [](sf::Color const& color, char const& c) -> char {
            auto bit = char(0);
            switch (c) {
                default:
                    bit = char(color.r & 1);
                    break;
                case 1:
                    bit = char(color.g & 1);
                    break;
                case 2:
                    bit = char(color.b & 1);
                    break;
            }
            return bit;
        };

        auto sizeOfMessage = int(0);
        // Pierwsze 4 Bajty - size of Message
        // Wskazanie na color R=0 G=1 B=2
        char colorPointer = char(0);
        {
            auto color = imgFile.getPixel(x, y);
            // som <> SizeOfMessage
            auto somBuffer = std::vector<char>(4);
            for(char& somElement : somBuffer) {
                for (char j = 0; j < 8; ++j, colorPointer++) {
                    if(colorPointer == 3){
                        ++x;
                        if(x == sizeOfImage.width + 1){
                            ++y;
                            x = 0;
                        }
                        color = imgFile.getPixel(x, y);
                        colorPointer = 0;
                    }
                    somElement <<= 1;
                    somElement |= getColor(color, colorPointer);
                }
                somElement ^= 53;
                somElement -= 53;
                sizeOfMessage <<= 8;
                sizeOfMessage += somElement;
            }
        }

        auto charMessageVictor = std::vector<char>(sizeOfMessage);
        {
            auto color = imgFile.getPixel(x, y);
            for(char& charElement : charMessageVictor) {
                for (char j = 0; j < 8; ++j, colorPointer++) {
                    if(colorPointer == 3){
                        ++x;
                        if(x == sizeOfImage.width){
                            ++y;
                            x = 0;
                        }
                        color = imgFile.getPixel(x, y);
                        colorPointer = 0;
                    }
                    charElement <<= 1;
                    charElement |= getColor(color, colorPointer);
                }
                charElement ^= 53;
            }
        }

        auto message = std::string(charMessageVictor.begin(), charMessageVictor.end());
        fmt::println("{}", message);

        return 0;
    }
}

auto f_decrypt(std::filesystem::path const& file) -> int {
    auto extension = getFileFormat(file);
    auto sizeOfImage = sizeOfImageHelper(file);

    auto returnCode = int(0);
    switch(extension){
        case FileFormat::BMP:
            returnCode = BMP::decrypt(file, sizeOfImage);
            break;
        case FileFormat::PNG:
        case FileFormat::SFML_Format:
            returnCode = sfmlImg::decrypt(file, sizeOfImage);
            break;
    }

    return returnCode;
}