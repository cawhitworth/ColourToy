#pragma once

#include <memory>
#include <list>
#include <mutex>
#include "Bitmap.h"
#include "ColourPicker.h"
#include "ConcurrentQueue.h"

class VersionThree
{
public:
    VersionThree(unsigned width, unsigned height);
    ~VersionThree();

    std::shared_ptr<Bitmap> Render();

private:
    void Worker();
    void AddPixel(unsigned x, unsigned y, unsigned c);
    void Enqueue(unsigned x, unsigned y);

    unsigned m_Width, m_Height;
    std::shared_ptr<Bitmap> m_Image;
    ColourPicker m_Picker;

    std::mutex m_PickerMutex;

    bool m_Abort;
    ConcurrentQueue<std::pair<unsigned, unsigned> > m_Queue;
};

