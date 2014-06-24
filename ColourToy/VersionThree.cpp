#include "stdafx.h"
#include "VersionThree.h"
#include "Colour.h"
#include <iostream>
#include <numeric>

VersionThree::VersionThree(unsigned w, unsigned h) : m_Queue(1000)
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
    m_Abort = false;

    std::vector<std::thread> workers;
    for (int i = 0; i < 8; i++)
    {
        workers.push_back(std::thread(&VersionThree::Worker, this));
    }

    std::this_thread::sleep_for(std::chrono::seconds(15));

    std::cout << "Done" << std::endl;

    m_Abort = true;

    while (!m_Queue.empty())
    {
        m_Queue.pop();
    }

    for (auto& t : workers)
    {
        t.join();
    }

    return m_Image;
}

void VersionThree::Worker()
{
    int iter = 0, plotted = 0;
    int lastPc = 0;
    while (!m_Queue.empty() && !m_Abort)
    {
        auto coord = m_Queue.pop();

        auto x = coord.first, y = coord.second;

        if (m_Image->Point(x, y) != Colour(0, 0, 0))
            continue;

        unsigned r = 0, g = 0, b = 0, count = 0;
        for (auto dx : { x - 1, x, x + 1 })
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
            std::unique_lock<std::mutex> lock(m_PickerMutex);
            r /= count; g /= count; b /= count;
            auto c = m_Picker.PickNearestTo(Colour(r, g, b));
            AddPixel(x, y, c);
            plotted++;
        }
    }

}

void VersionThree::AddPixel(unsigned x, unsigned y, unsigned c)
{
    m_Picker.Pick(c);
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

void VersionThree::Enqueue(unsigned x, unsigned y)
{
    m_Queue.push( std::pair<unsigned, unsigned>(x, y) );
}
