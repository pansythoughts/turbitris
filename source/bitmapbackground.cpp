#include "bitmapbackground.h"

void BitmapBackground::BMPBGInit()
{
    // the first 8 bases, except the first one, are reserved for the tiles of affine/tiled backgrounds.
    for (int i = 8; i < 32; i++)
    {
        if (BMP_and_tile_bases_screen_1[i].available)
        {
            map_base = i;
            for (int j = i; j < i + bases_used; j++)
                BMP_and_tile_bases_screen_1[j].available = false;
            break;
        }
    }

    if (screen == 0)
        id = bgInit(layer, BgType_Bmp16, size, map_base, 0);
    else
        id = bgInitSub(layer, BgType_Bmp16, size, map_base, 0);
}

void BitmapBackground::createBMPBG()
{
    dmaCopy((void *)pixels_buffer, (void *)bgGetGfxPtr(id), buffer_size);
}

BitmapBackground::BitmapBackground(int screen,
                                   int layer,
                                   uint16_t *pixels_buffer,
                                   int buffer_size,
                                   int height,
                                   int width,
                                   BgSize size)
{
    this->screen = screen;
    this->layer = layer;
    this->pixels_buffer = pixels_buffer;
    this->buffer_size = buffer_size;
    this->height = height;
    this->width = width;
    this->size = size;
    bases_used = int(buffer_size / 16000);
    BMPBGInit();
}