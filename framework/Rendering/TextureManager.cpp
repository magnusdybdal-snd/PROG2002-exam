// This is the TextureManager.cpp
#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

bool TextureManager::LoadTexture2D(const std::string& name, const std::string& filePath, GLuint unit, int channels, bool mipMap)
{
    int width, height, bpp;
    auto data = this->LoadTextureImage(filePath, width, height, bpp, channels);

    if (!data)
        {
        std::cerr << "Failed to load texture!" << std::endl;
        return false;
        }

    // Determine the correct OpenGL format based on channels for memory saving
    // By not converting everything to a 4 channel rgba
    GLenum internalFormat;
    GLenum format;

    switch(bpp) {
        case 1:
            internalFormat = GL_R8;
            format = GL_RED;
            break;
        case 2:
            internalFormat = GL_RG8;
            format = GL_RG;
            break;
        case 3:
            internalFormat = GL_RGB8;
            format = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            break;
        default:
            std::cerr << "Unsupported channel format!" << std::endl;
            this->FreeTextureImage(data);
            return false;        
    }

    //Generate a texture object and upload the loaded image to it.
    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + unit); // Texture Unit
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    if (mipMap)
        {
        glGenerateMipmap(GL_TEXTURE_2D);
        }

    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Texture texture;
    texture.mipMap = mipMap;
    texture.width = width;
    texture.height = height;
    texture.name = name;
    texture.filePath = filePath;
    texture.unit = unit;
    texture.type = Texture2D;

    this->Textures.push_back(texture);

    this->FreeTextureImage(data);

    return true;
}

bool TextureManager::LoadCubeMap(const std::string& name, const std::string& filePath, GLuint unit, int channels, bool mipMap)
{
    int width, height, bpp;
    auto data = this->LoadTextureImage(filePath, width, height, bpp, channels);

    if (!data)
    {
        std::cerr << "Failed to load texture!" << std::endl;
        return false;
    }

    //Generate a texture object and upload the loaded image to it.
    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + unit); // Texture Unit
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    // Determine the correct OpenGL format based on channels for memory saving
    // By not converting everything to a 4 channel rgba
    GLenum internalFormat;
    GLenum format;

    switch(bpp) {
        case 1:
            internalFormat = GL_R8;
            format = GL_RED;
            break;
        case 2:
            internalFormat = GL_RG8;
            format = GL_RG;
            break;
        case 3:
            internalFormat = GL_RGB8;
            format = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            break;
        default:
            std::cerr << "Unsupported channel format!" << std::endl;
            this->FreeTextureImage(data);
            return false;        
    }

    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }

    if (mipMap)
    {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    // Wrapping
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
    // Filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Texture texture;
    texture.mipMap = mipMap;
    texture.width = width;
    texture.height = height;
    texture.name = name;
    texture.filePath = filePath;
    texture.unit = unit;
    texture.type = CubeMap;

    this->Textures.push_back(texture);
    this->FreeTextureImage(data);

    return true;
}


GLuint TextureManager::GetUnitByName(const std::string& name) const
{
    for(const auto& texture: this->Textures)
    {
    if (!texture.name.compare(name))
        {
        return texture.unit;
        }
    }
    return -1;
}

unsigned char* TextureManager::LoadTextureImage(const std::string& filepath, int& width, int& height, int& bpp, int format) const
{
    return stbi_load(filepath.c_str(), &width, &height, &bpp, format);
}


void TextureManager::FreeTextureImage(unsigned char* data) const
{
    if (data)
        {
        stbi_image_free(data);
        }
}
