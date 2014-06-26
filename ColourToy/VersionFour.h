#pragma once
#include "Bitmap.h"
#include <memory>
#include "ColourPicker.h"

class VersionFour
{
public:
    VersionFour(unsigned width, unsigned height);
    ~VersionFour();
    std::shared_ptr<Bitmap> Render();

private:
    unsigned m_Height;
    unsigned m_Width;
    std::shared_ptr<Bitmap> m_Image;
    ColourPicker m_Picker;
};

