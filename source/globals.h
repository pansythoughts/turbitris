#pragma once

#include "libs.h"

extern volatile long frames;
extern int keys_held;
extern int keys_up;
extern int keys_down;

struct Base
{
    bool available = true;
    void *pointer = nullptr;
};

extern Base BMP_and_tile_bases_screen_1[32];
extern Base BMP_and_tile_bases_screen_2[32];

// ONLY THE FIRST 16 KB OR FIRST BASE IS RESERVED FOR AFFINE/TILED MAP BASES.
extern Base regular_map_bases_screen_1[8];
extern Base regular_map_bases_screen_2[8];

extern touchPosition touch;

u16 rgb8ToRgb565(const u8 r, const u8 g, const u8 b);