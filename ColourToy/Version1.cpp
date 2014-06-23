// ColourToy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <string>

#include "Version1.h"
#include "Bitmap.h"
#include "Colour.h"
#include "ColourPicker.h"

bool ProcessPixel(Bitmap&, ColourPicker&, unsigned, unsigned);

VersionOne::VersionOne(unsigned w, unsigned h)
{
    m_Width = w; m_Height = h;
    m_Image.reset(new Bitmap(w, h));
}

VersionOne::~VersionOne()
{

}


const std::shared_ptr<Bitmap> VersionOne::Render()
{
    m_Image->Plot(256, 256, Colour(0x01, 0x01, 0x01));
    int pass = 0;
    while (true)
    {
        pass++;
        std::cout << pass << std::endl;

        bool modified = false;
        for (int x = 0; x < m_Width; x++)
        {
            for (int y = 0; y < m_Height; y++)
            {
                modified |= ProcessPixel(x, y);
            }
        }

        if (!modified) break;

        for (int x = 0; x < m_Width; x++)
        {
            for (int y = 0; y < m_Height; y++)
            {
                auto c = m_Image->Point(x, y);
                SetA(c, 0xff);
                m_Image->Plot(x, y, c);
            }
        }
    }

    return m_Image;
}

bool VersionOne::ProcessPixel(unsigned x, unsigned y)
{
    auto c = m_Image->Point(x, y);

    if (c == Colour(0, 0, 0))
        return false;

    if (A(c) == 0)
        return false;

    bool modified = false;

    for (int dx = x - 1; dx < x + 2; dx++)
    {
        for (int dy = y - 1; dy < y + 2; dy++)
        {
            if (dx == x && dy == y) continue;
            if (dx < 0 || dx > m_Width - 1 || dy < 0 || dy > m_Height - 1) continue;
            if (m_Image->Point(dx, dy) != 0xff000000) continue;

            auto colour = m_ColourPicker.PickNearestTo(c);

            SetA(colour, 0);

            m_Image->Plot(dx, dy, colour);
            modified = true;
        }
    }

    return modified;
}