#include <fstream>
#include "fmt/core.h"
#include "fmt/ranges.h"
#include "functions.hpp"

auto f_encrypt(std::filesystem::path const& file, std::string message) -> int {
    auto sizeOfImage = sizeOfImageHelper(file);

    auto outputFile = std::filesystem::path(file.parent_path());
    outputFile /= "temp";
    outputFile += file.extension();
    auto output = std::fstream(outputFile, std::ios::binary | std::ios::out | std::ios::trunc);

    fmt::println("{}", outputFile.string());

    auto input = std::fstream(file, std::ios::binary | std::ios::in);
    auto buffer = std::vector<char>(512);

    if(file.extension() == ".bmp"){
        char startPosition;
        input.seekg(0xA);
        input.read(&startPosition,1);
        input.seekg(0);

//        // DEBUG START
//        fmt::println("{}",message.size());
//        for (auto& ch : message) {
//            ch = 0x4A;
//        }
//        // DEBUG END

        //Pierwsze 4 bajty -> długość wiadomości
        auto sizeOfMessage = int(message.size());
        // 4, bo 4 Bajty
        for (int i = 0; i < 4; ++i) {
            message.insert(i, 1, char(53 + ((sizeOfMessage >> (32 - 8 * (i + 1))) & 255)));
        }


        //XORowanie wiadomości
        for (auto& character : message) {
            character ^= 53;
        }

        {
            for (auto c : message) {
                fmt::print("{} ", int(c));
            }
            fmt::println("");
            for (auto c : message) {
                fmt::print("{0:b} ", int(c));
            }
            fmt::println("");
        }


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


    }
    else if(file.extension() == ".ppm") {

    }

    // */

    //Usunięcie pierwotnego pliku
    //Zmiana nazwy temp na pierwotną
    {

    }



    fmt::println("ENCRYPT!");
    return 12;
}

auto f_decrypt(std::filesystem::path const& file) -> int{
    {
        fmt::println("START");
    }
//     /*
    auto sizeOfImage = sizeOfImageHelper(file);

    auto input = std::fstream(file, std::ios::binary | std::ios::in);
    auto buffer = std::vector<char>(512);

    if(file.extension() == ".bmp"){
        char startPosition;
        input.seekg(0xA);
        input.read(&startPosition,1);
        input.seekg(startPosition);

        auto sizeOfMessage = int();
        auto lineIndex = int(0);
        auto paddings = sizeOfImage.width % 4;

        {
            auto somBuffer = std::vector<char>(4);
            auto readBufferSize = somBuffer.size() * 8;
            std::vector<char> readBuffer;
            if(readBufferSize <= sizeOfImage.width){
                readBuffer = std::vector<char>(readBufferSize);
            }
            else {
                readBuffer = std::vector<char>(readBufferSize + (paddings * sizeOfImage.height));
            }
            input.read(&readBuffer[0],ssize_t(readBuffer.size()));

            for (auto& element : readBuffer) {
                element &= 1;
            }

            for (int i = 0; i < somBuffer.size(); ++i) {
                for (int j = 0; j < 8; ++j) {
                    somBuffer.at(i) <<= 1;
                    somBuffer.at(i) |= readBuffer.at(i * 8 + j);
                }
                somBuffer.at(i) ^= 53;
                somBuffer.at(i) -= 53;
                sizeOfMessage <<= 8;
                sizeOfMessage += somBuffer.at(i);

//                if(lineIndex == sizeOfImage.width){
//                    lineIndex = 0;
//                    for (int j = 0; j < sizeOfImage.width % 4; ++j) {
//                        bufferIndex++;
//                    }
//                }
            }
        }

        {
            auto messageBuffer = std::vector<char>(sizeOfMessage);
            auto readBufferSize = sizeOfMessage * 8;
            std::vector<char> readBuffer;
            if(readBufferSize <= sizeOfImage.width){
                readBuffer = std::vector<char>(readBufferSize);
            }
            else {
                readBuffer = std::vector<char>(readBufferSize + (paddings * sizeOfImage.height));
            }

            input.read(&readBuffer[0],ssize_t(readBuffer.size()));

            for (auto& element : readBuffer) {
                element &= 1;
            }

            auto lineIndex = int(0);

            for (int i = 0; i < messageBuffer.size(); ++i) {
                for (int j = 0; j < 8; ++j) {
                    messageBuffer.at(i) <<= 1;
                    messageBuffer.at(i) |= readBuffer.at((i * 8) + j);
                }
                messageBuffer.at(i) ^= 53;
                lineIndex++;

                // Pilnowanie paddingów
                if(lineIndex == sizeOfImage.width){
                    lineIndex = 0;
                    for (int j = 0; j < sizeOfImage.width % 4; ++j) {
                        ++i;
                    }
                }
            }

            auto message = std::string(messageBuffer.begin(),messageBuffer.end());
            fmt::println("{}",message);
        }
    }
    return 15;
}

auto f_check(std::filesystem::path const& file, std::string const& message) -> int{
    auto numberOfChar = message.size();
    auto sizeOfImage = sizeOfImageHelper(file);
    auto numberOfBytes = sizeOfImage.width * sizeOfImage.height / 8;
    numberOfBytes *= 3;
    // 8 pierwszych bajtów zajętych przez długość wiadomości
    numberOfBytes -= 8;
    if(numberOfChar <= numberOfBytes){
        fmt::println("A message can be encrypt in file {}!", file.filename().string());
        return 0;
    }
    else {
        fmt::println("A message can not be encrypt in file {}!", file.filename().string());
        return 8;
    }

}