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
    }
}

namespace sfmlImg {
    auto decrypt(std::filesystem::path const& file, SizeOfImage const& sizeOfImage) -> int {

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