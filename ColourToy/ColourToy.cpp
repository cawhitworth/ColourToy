// ColourToy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <assert.h>
#include <string>

#include "lodepng.h"
#include "Bitmap.h"
#include "Colour.h"
#include "Version1.h"


const int WIDTH = 1024;
const int HEIGHT = 1024;
const int CHANNELS = 4;

const char* OUTPUT = "image";

void Sanity();

int _tmain(int argc, _TCHAR* argv[])
{
    Sanity();

    VersionOne v1(WIDTH, HEIGHT);

    auto image = v1.Render();

    std::string filename = OUTPUT;
    filename += ".png";
    auto err = lodepng::encode(filename, image->RawData(), WIDTH, HEIGHT);

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

    auto c4 = Colour(0xff, 0x00, 0x00);

    SetA(c4, 0);

    assert(c4 == 0xff);
}