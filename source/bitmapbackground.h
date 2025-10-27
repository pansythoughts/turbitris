#pragma once

#include "libs.h"
#include "globals.h"
#include "enums.h"

// BMP16
class BitmapBackground
{
public:
    int screen = 0;
    int layer = 0;
    int id = 0;
    int map_base = 0;
    int height = 0;
    int width = 0;
    uint16_t *pixels_buffer;
    int buffer_size;
    fixed32 scroll_x = 0;
    fixed32 scroll_y = 0;
    BgSize size = (BgSize)0;
    int bases_used = 0;

    void BMPBGInit();
    void createBMPBG();
    void deleteBMPBG();
    BitmapBackground(int screen,
                     int layer,
                     uint16_t *pixels_buffer,
                     int buffer_size,
                     int height,
                     int width,
                     BgSize size);
};