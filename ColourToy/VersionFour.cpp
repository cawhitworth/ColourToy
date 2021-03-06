#include "stdafx.h"
#include "VersionFour.h"
#include "Colour.h"
#include <iostream>
#include <thread>
#include <vector>

VersionFour::VersionFour(unsigned width, unsigned height)
{
    m_Width = width;
    m_Height = height;

    m_Image.reset(new Bitmap(width, height));
}


VersionFour::~VersionFour()
{
}

struct Point
{
    unsigned x, y, c;
};

std::shared_ptr<Bitmap> VersionFour::Render()
{

    for (int x = 0; x < m_Width; x++)
    {
        auto c = m_Picker.PickNearestTo(Colour(0xff, 0xff, 0xff));

        m_Image->Plot(x, 0, c);
    }

    for (int droplet = 0; droplet < 1000000; droplet++)
    {
        auto x = (rand() % (m_Width-2))+1;
        auto y = m_Height;
        bool finished = false;
        unsigned c;
        do
        {
            if ((c = m_Image->Point(x, y - 1)) != Colour(0, 0, 0))
                break;
            if ((c = m_Image->Point(x - 1, y - 1)) != Colour(0, 0, 0))
                break;
            if ((c = m_Image->Point(x + 1, y - 1)) != Colour(0, 0, 0))
                break;
            y--;
        } while (y > 0);

        if (y >= m_Height)
            continue;
        m_Image->Plot(x, y, m_Picker.PickNearestTo(c));
    }

    std::cout << "Finished droplets" << std::endl;

    auto worker = [=](int mod, int offset) {

        std::cout << offset << " - started" << std::endl;

        std::list<Point> points;
        int pass = 0;
        bool finished = false;
        while (!finished)
        {
            finished = true;
            for (unsigned y = offset; y < m_Height; y += mod)
            {
                for (unsigned x = 0; x < m_Width; x++)
                {
                    if (x % 64 == 0 && offset == 0)
                        std::cout << "\r" << x << "," << y << std::flush;

                    if (m_Image->Point(x, y) != Colour(0, 0, 0))
                        continue;

                    finished = false;

                    unsigned r = 0, g = 0, b = 0;
                    unsigned count = 0;

                    for (auto dx : { x - 1, x, x + 1 })
                    {
                        if (dx < 0 || dx >= m_Width) continue;
                        for (auto dy : { y - 1, y, y + 1 })
                        {
                            if (dy < 0 || dy >= m_Height) continue;

                            auto c = m_Image->Point(dx, dy);
                            if (c != Colour(0, 0, 0))
                            {
                                r += R(c); g += G(c); b += B(c); count++;
                            }
                        }
                    }

                    if (count == 0) continue;
                    r /= count; g /= count; b /= count;
                    auto c = m_Picker.PickNearestTo(Colour(r, g, b));
                    points.push_back({ x, y, c });
                }
            }


            std::cout << offset << " Pass " << pass++ << ": " << points.size() << " points " << std::endl;
            for (auto p : points)
            {
                m_Image->Plot(p.x, p.y, p.c);
            }

            points.clear();
        }
        std::cout << offset << " - done" << std::endl;
    };

    int numThreads = 4;
    std::vector<std::thread> threads;
    for (int t = 0; t < numThreads; t++)
    {
        threads.push_back(std::move(std::thread(worker, numThreads, t)));
    }

    for (auto& t : threads)
    {
        t.join();
    }

    return m_Image;
}
