#include "globals.h"

volatile long frames = 0;
int keys_held = 0;
int keys_up = 0;
int keys_down = 0;

Base BMP_and_tile_bases_screen_1[32];
Base BMP_and_tile_bases_screen_2[32];
// ONLY THE FIRST 16 KB OR FIRST BASE ARE RESERVED FOR AFFINE/TILED MAP BASES.
// check this memory layout if confused:
// https://mtheall.com/vram.html#T2=3&RNT2=32&MB2=0&TB2=1&S2=1&T3=6&MB3=8&S3=1
Base regular_map_bases_screen_1[8];
Base regular_map_bases_screen_2[8];
// maybe this could go somewhere else, but eh, for now here.
u16 rgb8ToRgb565(const u8 r, const u8 g, const u8 b)
{
    u16 red = (r >> 3);
    u16 green = (g >> 3);
    u16 blue = (b >> 3);
    u16 color = (1 << 15) | (red) | (green << 5) | (blue << 10);
    return color;
}
