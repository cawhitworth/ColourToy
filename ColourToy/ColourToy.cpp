// ColourToy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "lodepng.h"
#include "Bitmap.h"
#include "Colour.h"
#include <assert.h>

const int WIDTH = 512;
const int HEIGHT = 512;
const int CHANNELS = 4;

const char* OUTPUT = "image.png";

void Sanity();

int _tmain(int argc, _TCHAR* argv[])
{
    Sanity();

    auto usedColours = new bool[256 * 256 * 256];

    Bitmap image(WIDTH, HEIGHT);

    image.Plot(10, 10, Colour(0xff, 0, 0xff));



    auto err = lodepng::encode(OUTPUT, image.RawData(), WIDTH, HEIGHT);

    return 0;
}

void Sanity()
{
    assert(Colour(0xff, 0, 0) == 0xff0000ff);
    
    auto c1 = Colour(0xff, 0x00, 0x00);
    auto c2 = Colour(0xfe, 0x00, 0x00);
    auto c3 = Colour(0xff, 0x01, 0x00);

    assert(Distance(c1, c2) == Distance(c1, c3));

    assert(Distance(c1, c2) < Distance(c2, c3));
}