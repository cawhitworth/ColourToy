#include "stdafx.h"
#include "ColourPicker.h"
#include "Colour.h"

ColourPicker::ColourPicker()
{
    m_usedColour = new bool[256 * 256 * 256];
    for (int i = 0; i < 256 * 256 * 256; i++)
    {
        m_usedColour[i] = false;
    }
}

ColourPicker::~ColourPicker()
{
    delete [] m_usedColour;
}

void ColourPicker::Pick(unsigned colour)
{
    m_usedColour[colour & 0x00ffffff] = true;
}

unsigned ColourPicker::PickNearestTo(unsigned colour)
{
    auto idx = colour & 0x00ffffff;

    int distance = 1;
    unsigned newIdx;

    while (true)
    {
        for (int dr = R(idx) - distance; dr < R(idx) + distance; dr++)
        {
            if (dr < 0 || dr > 255) continue;

            for (int dg = G(idx) - distance; dg < G(idx) + distance; dg++)
            {
                if (dg < 0 || dg > 255) continue;

                for (int db = B(idx) - distance; db < B(idx) + distance; db++)
                {
                    if (db < 0 || db > 255) continue;

                    newIdx = Colour(dr, dg, db) & 0x00ffffff;

                    if (!m_usedColour[newIdx])
                    {
                        m_usedColour[newIdx] = true;
                        return newIdx | 0xff000000;
                    }
                }
            }
        }
        distance++;
    }
}