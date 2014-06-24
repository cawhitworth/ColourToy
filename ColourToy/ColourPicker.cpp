#include "stdafx.h"
#include "ColourPicker.h"
#include "Colour.h"
#include <vector>
#include <iostream>
#include <algorithm>

#define THRESHOLD 40000

ColourPicker::ColourPicker()
{
    m_usedColour = new bool[256 * 256 * 256];
    for (int i = 0; i < 256 * 256 * 256; i++)
    {
        m_usedColour[i] = false;
        m_freeCount++;
    }
}

ColourPicker::~ColourPicker()
{
    delete [] m_usedColour;
}

void ColourPicker::Pick(unsigned colour)
{
    m_usedColour[colour & 0x00ffffff] = true;
    m_freeCount--;
}

unsigned ColourPicker::PickNearestToAlt(unsigned colour)
{
    if (m_AltCandidates.begin() == m_AltCandidates.end())
    {
        std::cout << "Switched to alt picker" << std::endl;
        for (int idx = 0; idx < 256 * 256 * 256; idx++)
        {
            if (!m_usedColour[idx])
                m_AltCandidates.push_back(idx);
        }
    }

    unsigned minDist = 0x7fffffff;
    auto best = m_AltCandidates.begin();
    for (auto itr = m_AltCandidates.begin(); itr != m_AltCandidates.end(); itr++)
    {
        auto c = *itr;
        auto d = Distance(colour, c);
        if (d < minDist)
        {
            minDist = d;
            best = itr;
        }
    }

    auto col = *best;
    m_AltCandidates.erase(best);

    m_usedColour[col] = true;
    m_freeCount--;
    return col | 0xff000000;
}

unsigned ColourPicker::PickNearestTo(unsigned colour)
{
    if (m_freeCount < THRESHOLD)
    {
        return PickNearestToAlt(colour);
    }
    auto idx = colour & 0x00ffffff;

    int distance = 1;
    unsigned newIdx;

    std::vector<unsigned> candidates;
    
    int scannedThrough = 0;

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
                        candidates.push_back(newIdx);
                    }
                }
            }
        }

        auto s = candidates.size();
        scannedThrough += s;

        if (s > 0)
        {
            auto c = s / 2;

            newIdx = candidates[c];
            m_usedColour[newIdx] = true;
            m_freeCount--;
            if (scannedThrough > m_maxScannedThrough)
                m_maxScannedThrough = scannedThrough;
            return newIdx | 0xff000000;
        }

        distance++;
    }
}