#include "stdafx.h"
#include "VersionThree.h"
#include "Colour.h"
#include <iostream>
#include <numeric>
#include <future>

VersionThree::VersionThree(unsigned w, unsigned h) : m_Queue(32)
{
    m_Width = w; m_Height = h;
    m_Image.reset(new Bitmap(w, h));
}

VersionThree::~VersionThree()
{
}

std::shared_ptr<Bitmap> VersionThree::Render()
{
    AddPixel(m_Width / 2, m_Height / 2, Colour(0xff, 0xff, 0xff));

    Worker();

    return m_Image;
}

void VersionThree::Worker()
{
    int iter = 0, plotted = 0, weird = 0, overdraw = 0, race = 0;
    int lastPc = 0;
    while (true)
    {
        auto pc = (int) ((plotted * 100.f) / (m_Width * m_Height));
        if (pc > lastPc)
        {
            std::cout << pc << std::endl;
            lastPc = pc;
        }

        if (plotted == m_Width * m_Height)
        {
            std::cout << "Break" << std::endl;
            std::cout << race << std::endl;
            std::cout << weird << std::endl;

            break;
        }

        auto coord = m_Queue.pop();

        auto x = coord.first, y = coord.second;

        if (m_Image->Point(x, y) != 0)
            continue;


        unsigned r = 0, g = 0, b = 0, count = 0;
        for (auto dx : { x - 1, x, x + 1 })
        {
            if (dx < 0 || dx > m_Width - 1) continue;
            for (auto dy : { y - 1, y, y + 1 })
            {
                if (dy<0 || dy > m_Height - 1) continue;

                auto col = m_Image->Point(dx, dy);
                if (col != Colour(0, 0, 0) && col != ColourA(0,0,0,0))
                {
                    r += R(col); g += G(col); b += B(col); count++;
                }
            }
        }
        if (count > 0)
        {
            std::unique_lock<std::mutex> mlock(m_Mutex);
            if (m_Image->Point(x, y) == 0)
            {
                r /= count; g /= count; b /= count;
                auto c = m_Picker.PickNearestTo(Colour(r, g, b));
                AddPixel(x, y, c);
                plotted++;
            }
            else
            {
                race++;

            }
        }
        else
        {
            weird++;
        }
    }

}

void VersionThree::AddPixel(unsigned x, unsigned y, unsigned c)
{
    m_Image->Plot(x, y, c);

    auto enqueue = [=]() {

        for (auto dx : { x - 1, x, x + 1 })
        {
            if (dx < 0 || dx > m_Width - 1) continue;
            for (auto dy : { y - 1, y, y + 1 })
            {
                if (dy<0 || dy > m_Height - 1) continue;

                if (m_Image->Point(dx, dy) == Colour(0, 0, 0))
                {
                    m_Image->Plot(dx, dy, ColourA(0, 0, 0, 0));
                    m_Queue.push( std::pair<unsigned, unsigned>(dx, dy) );
                }
            }
        }
    };

    std::async(enqueue);
}