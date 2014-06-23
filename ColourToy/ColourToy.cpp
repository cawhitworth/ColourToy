// ColourToy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <string>

#include "lodepng.h"
#include "Bitmap.h"
#include "Colour.h"
#include "ColourPicker.h"


const int WIDTH = 512;
const int HEIGHT = 512;
const int CHANNELS = 4;

const char* OUTPUT = "image";

void Sanity();
bool ProcessPixel(Bitmap&, ColourPicker&, unsigned, unsigned);

int _tmain(int argc, _TCHAR* argv[])
{
    Sanity();

    ColourPicker p;
    Bitmap image(WIDTH, HEIGHT);

    image.Plot(256,256, Colour(0x01, 0x01, 0x01));

    for (int pass = 0; pass < 400; pass++)
    {
        std::cout << pass << std::endl;

        bool modified = false;
        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                modified |= ProcessPixel(image, p, x, y);
            }
        }

        if (!modified) break;

        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                auto c = image.Point(x, y);
                SetA(c, 0xff);
                image.Plot(x, y, c);
            }
        }

        std::string filename = OUTPUT;
        filename += std::to_string(pass);
        filename += ".png";

        auto err = lodepng::encode(filename.c_str(), image.RawData(), WIDTH, HEIGHT);
    }

    std::string filename = OUTPUT;
    filename += ".png";
    auto err = lodepng::encode(filename, image.RawData(), WIDTH, HEIGHT);

    return 0;
}


bool ProcessPixel(Bitmap& image, ColourPicker& p, unsigned x, unsigned y)
{
    auto c = image.Point(x, y);
    
    if (c == Colour(0, 0, 0))
        return false;

    if (A(c) == 0)
        return false;

    bool modified = false;

    for (int dx = x-1; dx < x+2; dx++)
    {
        for (int dy = y-1; dy < y+2; dy++)
        {
            if (dx == x && dy == y) continue;
            if (dx < 0 || dx > WIDTH - 1 || dy < 0 || dy > HEIGHT - 1) continue;
            if (image.Point(dx, dy) != 0xff000000) continue;

            auto colour = p.PickNearestTo(c);

            SetA(colour, 0);

            image.Plot(dx, dy, colour);
            modified = true;
        }
    }

    return modified;
}

void Sanity()
{
    assert(Colour(0xff, 0, 0) == 0xff0000ff);
    
    auto c1 = Colour(0xff, 0x00, 0x00);
    auto c2 = Colour(0xfe, 0x00, 0x00);
    auto c3 = Colour(0xff, 0x01, 0x00);

    assert(Distance(c1, c2) == Distance(c1, c3));

    assert(Distance(c1, c2) < Distance(c2, c3));

    auto c4 = Colour(0xff, 0x00, 0x00);

    SetA(c4, 0);

    assert(c4 == 0xff);
}