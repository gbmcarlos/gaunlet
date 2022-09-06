#pragma once

#include "gaunlet/pch.h"

namespace gaunlet::Graphics {

    class Image {

    public:

        explicit Image(const char* path, bool flipVertically);
        ~Image();

        inline void* getData() {return m_data; }
        inline int getWidth() const {return m_width; }
        inline int getHeight() const {return m_height; }
        inline int getChannels() const {return m_channels; }

    private:
        void* m_data = nullptr;
        int m_width = 0;
        int m_height = 0;
        int m_channels = 0;

    };

}