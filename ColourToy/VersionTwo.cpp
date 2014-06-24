#include "stdafx.h"
#include "VersionTwo.h"
#include "Colour.h"
#include <iostream>
#include <numeric>

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
    AddPixel(m_Width / 2, m_Height / 2, Colour(0x01, 0x01, 0x01));

    int iter = 0, plotted= 0;

    int lastPc = 0;
    while (!m_Queue.empty())
    {
        auto pc = (int) ((plotted * 100.f) / (m_Width * m_Height));
        if (pc > lastPc)
        {
            std::cout << pc << std::endl;
            lastPc = pc;
        }

        auto coord = m_Queue.front();
        m_Queue.pop_front();
        auto x = coord.first, y = coord.second;

        if (m_Image->Point(x, y) != Colour(0, 0, 0))
            continue;

        unsigned r = 0, g = 0, b = 0, count = 0;
        for (auto dx : { x - 1, x, x + 1 } )
        {
            if (dx < 0 || dx > m_Width - 1) continue;
            for (auto dy : { y - 1, y, y + 1 })
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
    m_Image->Plot(x, y, c);

    for (auto dx : { x - 1, x, x + 1 })
    {
        if (dx < 0 || dx > m_Width - 1) continue;
        for (auto dy : { y - 1, y, y + 1 })
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
