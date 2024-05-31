#include "functions.hpp"

namespace BMP {
    auto decrypt(std::filesystem::path const& file, SizeOfImage const& sizeOfImage) -> void {
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

        // Pierwszy bufor
        auto readBufferIndex = int(0);
        input.read(&buffer[0], bufferSize);

        //Lambdy
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

        // Pierwsze 4 Bajty -> znalezienie rozmiaru wiadomości
        { // SizeOfMessage (som)
            auto somBuffer = std::vector<char>(4);
            for(char& somElement : somBuffer) {
                for (int j = 0; j < 8; ++j, ++readBufferIndex) {
                    somElement <<= 1;
                    somElement = static_cast<char>(somElement | buffer.at(readBufferIndex));
                }
                somElement ^= 53;
                somElement -= 53;
                sizeOfMessage <<= 8;
                sizeOfMessage += somElement;
                lineIndex++;

                paddingHelper(); // Pilnowanie paddingów
            }
        }

        checkMessageSize(sizeOfMessage, sizeOfImage);
        auto messageBuffer = std::vector<char>(sizeOfMessage);
        auto messageBufferIndex = int(0);
        auto byteVisor = int(0);

        auto nextBuffer = [&]() mutable -> bool {
            for ( ; readBufferIndex < input.gcount(); ++readBufferIndex){
                messageBuffer.at(messageBufferIndex) <<= 1;
                messageBuffer.at(messageBufferIndex) = static_cast<char>(messageBuffer.at(messageBufferIndex) | buffer.at(readBufferIndex));

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

        // Pierwszy przebieg do rozmiaru bufora.
        // Jeśli wiadomość jest mniejsza to jedyny przebieg
        nextBuffer();

        while(messageBufferIndex < sizeOfMessage){
            input.read(&buffer[0], bufferSize);
            readBufferIndex = int(0);
            bufferANDHelper();
            if(nextBuffer()){
                break;
            }
        }

        xorContent(messageBuffer, 53);

        auto message = std::string(messageBuffer.begin(),messageBuffer.end());
        fmt::println("{}",message);
    }
}

namespace sfmlImg {
    auto decrypt(std::filesystem::path const& file, SizeOfImage const& sizeOfImage) -> int {
        auto imgFile = sf::Image();
        imgFile.loadFromFile(file.string());
        auto x = int(0);
        auto y = int(0);

        auto getColor = [](sf::Color const& color, char const& c) -> int {
            switch (c) {
                default:
                    return color.r & 1;
                case 1:
                    return color.g & 1;
                case 2:
                    return color.b & 1;
            }
        };

        auto sizeOfMessage = int(0);
        // Pierwsze 4 Bajty - size of Message
        // Wskazanie na color R=0 G=1 B=2
        char colorPointer = char(0);
        auto color = imgFile.getPixel(x, y);

        auto getByte = [&](char& item) -> void {
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
                item <<= 1;
                item = static_cast<char>(item | getColor(color,colorPointer));
            }
            item ^= 53;
        };

        // som <> SizeOfMessage
        auto somBuffer = std::vector<char>(4);
        for(char& somElement : somBuffer) {
            getByte(somElement);
            somElement -= 53;
            sizeOfMessage <<= 8;
            sizeOfMessage += somElement;
        }
        checkMessageSize(sizeOfMessage, sizeOfImage);
        auto charMessageVictor = std::vector<char>(sizeOfMessage);
        for(char& charElement : charMessageVictor) {
           getByte(charElement);
        }

        auto message = std::string(charMessageVictor.begin(), charMessageVictor.end());
        fmt::println("{}", message);

        return 0;
    }
}

auto f_decrypt(std::filesystem::path const& file) -> void {
    auto extension = getFileFormat(file);
    auto sizeOfImage = sizeOfImageHelper(file);
    switch(extension){
        case FileFormat::BMP:
            BMP::decrypt(file, sizeOfImage);
            break;
        case FileFormat::SFML_Format:
            sfmlImg::decrypt(file, sizeOfImage);
            break;
    }
}