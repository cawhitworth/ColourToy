#pragma once

#include <memory>
#include <list>
#include "Bitmap.h"
#include "ColourPicker.h"

class VersionTwo
{
public:
    VersionTwo(unsigned width, unsigned height);
    ~VersionTwo();

    std::shared_ptr<Bitmap> Render();

private:
    void AddPixel(unsigned x, unsigned y, unsigned c);
    void Enqueue(unsigned x, unsigned y);

    unsigned m_Width, m_Height;
    std::shared_ptr<Bitmap> m_Image;
    ColourPicker m_Picker;

    std::list<std::pair<unsigned, unsigned> > m_Queue;
};

