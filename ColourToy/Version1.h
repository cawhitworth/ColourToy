#pragma once
#include <memory>
#include "Bitmap.h"
#include "ColourPicker.h"

class VersionOne
{
public:
    VersionOne(unsigned height, unsigned width);
    ~VersionOne();

    const std::shared_ptr<Bitmap> Render();

    bool ProcessPixel(unsigned x, unsigned y);

private:
    unsigned m_Height, m_Width;
    std::shared_ptr<Bitmap> m_Image;
    ColourPicker m_ColourPicker;
};