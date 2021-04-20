#include "Image.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cctype>

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
        static_assert(sizeof(Raw) == sizeof(Fields), "Sizes don't match");
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
            << "Invalid format signature." << std::endl <<
            "Expected: 0x42, 0x4D" << std::endl <<
            "Actual:   " << std::hex << std::showbase
            << static_cast<int>(header.fields.B) << ", "
            << static_cast<int>(header.fields.M);
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
            reinterpret_cast<const GL::Pixel::BGR<GLubyte>*>(data.data()));
    } else {
        std::stringstream ss;
        ss << __func__ << '(' << path << ')' << std::endl
            << "Unsupported bitCount: " << header.fields.info.bitCount;
        throw std::runtime_error(ss.str());
    }

    return result;
}

GL::Texture2D LoadDDS(const char* path, uint32_t* mipmapCount)
{
    static constexpr uint32_t FOURCC_DXT1 = 0x31545844; // Equivalent to "DXT1" in ASCII
    static constexpr uint32_t FOURCC_DXT3 = 0x33545844; // Equivalent to "DXT3" in ASCII
    static constexpr uint32_t FOURCC_DXT5 = 0x35545844; // Equivalent to "DXT5" in ASCII

    static constexpr std::size_t headerSize = 128;
    union DDSHeader {
        struct Fields {
            char signature[4];
            uint32_t unknown_0[2];
            uint32_t height;
            uint32_t width;
            uint32_t linearSize;
            uint32_t unknown_1;
            uint32_t mipmapCount;
            uint32_t unknown_3[13];
            uint32_t fourCC;
            uint32_t unknown_4[10];
        } fields;
        char raw[headerSize];
        static_assert(sizeof(Fields) == headerSize, "Sizes don't match");
    } header;

    std::ifstream file(path, std::ios::in | std::ios::binary);

    if(!file.is_open()) {
        std::stringstream ss;
        ss << __func__ << '(' << path << ')' << std::endl
            << "Failed to open file";
        throw std::runtime_error(ss.str());
    }

    file.read(header.raw, headerSize);
    auto gcount = file.gcount();
    if(gcount != headerSize) {
        std::stringstream ss;
        ss << __func__ << '(' << path << ')' << std::endl
            << "Failed to read file header. Expected " << headerSize << " bytes, actual size: " << gcount;
        throw std::runtime_error(ss.str());
    }

    if(strncmp(header.fields.signature, "DDS ", 4) != 0) {
        std::stringstream ss;
        ss << __func__ << '(' << path << ')' << std::endl
            << "Invalid format signature." << std::endl
            << "Expected: 0x44, 0x44, 0x53, 0x20"
            << "Actual:   " << std::hex << std::showbase
            << static_cast<int>(header.fields.signature[0]) << ", "
            << static_cast<int>(header.fields.signature[1]) << ", "
            << static_cast<int>(header.fields.signature[2]) << ", "
            << static_cast<int>(header.fields.signature[3]);
        throw std::runtime_error(ss.str());
    }

    const auto bufsize =
    (header.fields.mipmapCount > 1)
    ? (header.fields.linearSize * 2)
    : header.fields.linearSize;

    std::vector<char> data(bufsize);

    file.read(data.data(), bufsize);
    gcount = file.gcount();
    if(gcount != bufsize) {
        std::stringstream ss;
        ss << __func__ << '(' << path << ')' << std::endl
            << "Failed to read file data. Expected " << bufsize << " bytes, actual size: " << gcount;
        throw std::runtime_error(ss.str());
    }

    file.close();

    unsigned int components;
    unsigned int blockSize;
    GL::S3TCFormat format;
    switch(header.fields.fourCC) {
    case FOURCC_DXT1:
        format = GL::COMPRESSED_RGBA_S3TC_DXT1_EXT;
        components = 3;
        blockSize = 8;
        break;
    case FOURCC_DXT3:
        format = GL::COMPRESSED_RGBA_S3TC_DXT3_EXT;
        components = 4;
        blockSize = 16;
        break;
    case FOURCC_DXT5:
        format = GL::COMPRESSED_RGBA_S3TC_DXT5_EXT;
        components = 4;
        blockSize = 16;
        break;
    default:
        {
            std::stringstream ss;
            ss << __func__ << '(' << path << ')' << std::endl
                << "Unsupported FOURCC: " << std::hex << std::showbase << header.fields.fourCC;
            throw std::runtime_error(ss.str());
        }
    }

    GL::Texture2D result;
    result.Bind();

    unsigned int offset = 0;

    for (unsigned int level = 0; level < header.fields.mipmapCount && (header.fields.width || header.fields.height); ++level)
    {
        unsigned int size =
            ((header.fields.width  + 3) / 4) *
            ((header.fields.height + 3) / 4) * blockSize;
        GL::Texture2D::CompressedImage2D(level, format, header.fields.width, header.fields.height, size, &data[offset]);

        offset += size;
        header.fields.width  /= 2;
        header.fields.height /= 2;
    }
    
    if(mipmapCount) {
        *mipmapCount = header.fields.mipmapCount;
    }

    return result;
}

static GL::Texture2D LoadBMP(const std::string& s, uint32_t* mipmapCount, bool* invertV)
{
    if(mipmapCount) {
        *mipmapCount = 1;
    }
    if(invertV) {
        *invertV = false;
    }

    return LoadBMP(s.data());
}

static GL::Texture2D LoadDDS(const std::string& s, uint32_t* mipmapCount, bool* invertV)
{
    if(invertV) {
        *invertV = true;
    }

    return LoadDDS(s.data(), mipmapCount);
}

struct ExtentionLoad {
    const char* extention;
    GL::Texture2D (*func) (const std::string& s, uint32_t* mipmapCount, bool* invertV);
};

static const ExtentionLoad loaders[] =
{
    {"dds", LoadDDS},
    {"bmp", LoadBMP}
};

GL::Texture2D Load(const std::string& s, uint32_t* mipmapCount, bool* invertV)
{
    auto idx = s.find_last_of('.');
    if(idx != std::string::npos) {
        auto ext = s.substr(idx + 1);
        for(auto& ch : ext) {
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }

        for(auto& l : loaders) {
            if(ext.compare(l.extention) == 0) {
                return l.func(s, mipmapCount, invertV);
            }
        }
    }

    std::stringstream ss;

    for(auto& l : loaders) {
        try {
            return l.func(s, mipmapCount, invertV);
        } catch (const std::exception& e) {
            ss << std::endl << e.what();
        }
    }

    throw std::runtime_error(ss.str());
}

} // namespace Image
