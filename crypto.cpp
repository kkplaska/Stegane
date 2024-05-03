#include <fstream>
#include "fmt/core.h"
#include "fmt/ranges.h"
#include "functions.hpp"

auto f_encrypt(std::filesystem::path const& file, std::string message) -> int{
    {
        fmt::println("START");
    }
//     /*
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

        //Pierwsze 8 bajtów -> długość wiadomości
        auto sizeOfMessage = message.size();
        for (int i = 0; i < 8; ++i) {
            message.insert(i, 1, char(53 + ((sizeOfMessage >> (64 - 8 * (i + 1))) & 255)));
        }
        sizeOfMessage ^= 53;

//        {
//            for (auto c : message) {
//                fmt::print("{}", int(c));
//            }
//            fmt::println("");
//        }

        //XORowanie wiadomości
        for (auto& character : message) {
            character ^= 53;
        }


        // Przepisywanie nagłówka
        {
            auto miniBuffer = std::vector<char>(startPosition);
            input.read(&miniBuffer[0], startPosition);
            output.write(&miniBuffer[0], startPosition);
        }

        {
            input.read(&buffer[0],buffer.size());

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
                    character << 1;
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
                    output.write(&buffer[0],buffer.size());
                    bufferIndex = 0;
                    input.read(&buffer[0],buffer.size());
                }
            }
            output.write(&buffer[0],buffer.size());

            if(buffer.size() != input.gcount()){
                auto tmp = std::vector<char>(buffer.begin(), buffer.begin() + input.gcount());
                buffer = tmp;
                output.write(&buffer[0],input.gcount());
            }
            else {
                while(input.read(&buffer[0],buffer.size())){
                    output.write(&buffer[0],input.gcount());
                }

                if(buffer.size() != input.gcount()){
                    auto tmp = std::vector<char>(buffer.begin(), buffer.begin() + input.gcount());
                    buffer = tmp;
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
    fmt::println("DECRYPT!");
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