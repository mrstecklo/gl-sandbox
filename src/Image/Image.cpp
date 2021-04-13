#include "Image.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace Image {

GL::Texture2D LoadBMP(const char* path)
{
    static constexpr size_t paddingSize = 2;
    union BMPHeader {
        struct Fields {
            char padding[paddingSize];
            char B;
            char M;
            struct File {
                uint32_t size;
                uint16_t reserved_0;
                uint16_t reserved_1;
                uint32_t dataOffset;
            } file;
            static_assert(sizeof(File) == 12, "Invalid padding");
            struct Info {
                uint32_t infoSize;
                uint32_t width;
                uint32_t height;
                uint16_t planes;
                uint16_t bitCount;
                uint32_t compression;
                uint32_t sizeImage;
                uint32_t xppm;
                uint32_t yppm;
                uint32_t clrUsed;
                uint32_t clrImportant;
            } info;
            static_assert(sizeof(Info) == 40, "Invalid padding");
        } fields;
        struct Raw {
            char padding[paddingSize];
            char data[sizeof(Fields) - paddingSize];
        } raw;
        static_assert(sizeof(Fields) == (paddingSize + 2 + sizeof(Fields::File) + sizeof(Fields::Info)), "Invalid padding");
    } header;

    static constexpr size_t headerSize = sizeof(BMPHeader::Raw::data);

    std::ifstream file(path, std::ios::in | std::ios::binary);

    if(!file.is_open()) {
        std::stringstream ss;
        ss << __func__ << '(' << path << ')' << std::endl
            << "Failed to open file";
        throw std::runtime_error(ss.str());
    }

    file.read(header.raw.data, headerSize);
    auto gcount = file.gcount();
    if(gcount != headerSize) {
        std::stringstream ss;
        ss << __func__ << '(' << path << ')' << std::endl
            << "Failed to read file header. Expected " << headerSize << " bytes, actual size: " << gcount;
        throw std::runtime_error(ss.str());
    }

    if(header.fields.B != 'B' || header.fields.M != 'M') {
        std::stringstream ss;
        ss << __func__ << '(' << path << ')' << std::endl
            << "Invalid format signature. Expected 0x42, 0x4D, actual " << std::hex << std::showbase
            << static_cast<int>(header.fields.B) << ", " << static_cast<int>(header.fields.M);
        throw std::runtime_error(ss.str());
    }

    if(header.fields.info.bitCount == 0) {
        header.fields.info.bitCount = 24;
    }

    if(header.fields.info.sizeImage == 0) {
        header.fields.info.sizeImage = header.fields.info.width * header.fields.info.height * header.fields.info.bitCount / 8;
    }

    if(header.fields.file.dataOffset == 0) {
        header.fields.file.dataOffset = headerSize;
    }

    std::vector<char> data(header.fields.info.sizeImage);

    file.seekg(header.fields.file.dataOffset);
    file.read(data.data(), header.fields.info.sizeImage);
    gcount = file.gcount();
    if(gcount != header.fields.info.sizeImage) {
        std::stringstream ss;
        ss << __func__ << '(' << path << ')' << std::endl
            << "Failed to read file data. Expected " << header.fields.info.sizeImage << " bytes, actual size: " << gcount;
        throw std::runtime_error(ss.str());
    }

    file.close();

    GL::Texture2D result;
    result.Bind();

    if(header.fields.info.bitCount == 24) {
        GL::Texture2D::Image2D(
            0,
            GL::RGB8,
            header.fields.info.width,
            header.fields.info.height,
            reinterpret_cast<const GL::Pixel::RGB<GLubyte>*>(data.data()));
    } else {
        std::stringstream ss;
        ss << __func__ << '(' << path << ')' << std::endl
            << "Unsupported bitCount: " << header.fields.info.bitCount;
        throw std::runtime_error(ss.str());
    }

    return result;
}

} // namespace Image