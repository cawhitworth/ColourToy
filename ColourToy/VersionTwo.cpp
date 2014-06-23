#include "stdafx.h"
#include "VersionTwo.h"
#include "Colour.h"
#include <iostream>


VersionTwo::VersionTwo(unsigned w, unsigned h)
{
    m_Width = w; m_Height = h;
    m_Image.reset(new Bitmap(w, h));
}

VersionTwo::~VersionTwo()
{
}

std::shared_ptr<Bitmap> VersionTwo::Render()
{
    AddPixel(m_Width / 2, m_Height / 2, Colour(0x01, 0x00, 0x00));

    int iter = 0, plotted= 0;

    while (!m_Queue.empty())
    {
        auto coord = m_Queue.front();
        m_Queue.pop_front();
        auto x = coord.first, y = coord.second;

        if (m_Image->Point(x, y) != Colour(0, 0, 0))
            continue;

        unsigned r = 0, g = 0, b = 0, count = 0;
        for (int dx = x - 1; dx < x + 2; dx++)
        {
            if (dx < 0 || dx > m_Width - 1) continue;
            for (int dy = y - 1; dy < y + 2; dy++)
            {
                if (dy<0 || dy > m_Height - 1) continue;
                auto col = m_Image->Point(dx, dy);
                if (col != Colour(0, 0, 0))
                {
                    r += R(col); g += G(col); b += B(col); count++;
                }
            }
        }
        if (count > 0)
        {
            r /= count; g /= count; b /= count;
            auto c = m_Picker.PickNearestTo(Colour(r, g, b));
            AddPixel(x, y, c);
            plotted++;
        }
    }

    return m_Image;
}

void VersionTwo::AddPixel(unsigned x, unsigned y, unsigned c)
{
    m_Picker.Pick(c);
    m_Image->Plot(x, y, c);
    for (int dx = x - 1; dx < x + 2; dx++)
    {
        if (dx < 0 || dx > m_Width - 1) continue;
        for (int dy = y - 1; dy < y + 2; dy++)
        {
            if (dy<0 || dy > m_Height - 1) continue;

            if (m_Image->Point(dx, dy) == Colour(0, 0, 0))
            {
                Enqueue(dx, dy);
            }
        }
    }
}

void VersionTwo::Enqueue(unsigned x, unsigned y)
{
    m_Queue.emplace_back(x, y);
}