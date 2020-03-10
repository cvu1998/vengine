#pragma once

#include <string>

#include "Utility.h"

class Texture
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;

public:
    Texture(const std::string filepath);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unBind() const;

    inline int getHeight() const { return m_Height; }
    inline int getWidth() const { return m_Width; }
};

