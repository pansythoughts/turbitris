#pragma once

#include "gfx/blocks.h"
#include "enums.h"
#include "libs.h"
#include "globals.h"
#include "input.h"

class AffineBackground
{
private:
    fixed32 scroll_x = 0;
    fixed32 scroll_y = 0;
    fixed32 scale_x = inttof32(1);
    fixed32 scale_y = inttof32(1);
    fixed32 angle = 0;
    int scale_direction = 1;
    int screen = 0;

    BgSize size = (BgSize)0;

    int layer = 0;

public:
    int id = 0;
    int tile_base = 0;
    int map_base = 0;
    const unsigned char *tiles = nullptr;
    const unsigned short *pal = nullptr;
    const unsigned short *map = nullptr;
    int tile_size = 0;
    int pal_size = 0;
    int map_size = 0;
    u8 map_buffer[1024];
    volatile u8 *mapptr = nullptr;

    struct ShakeData
    {
        bool shaking = false;
        fixed32 current_shake_amplitude_x = 0;
        fixed32 current_shake_amplitude_y = 0;
        int target_shake_amplitude_x = 0;
        int target_shake_amplitude_y = 0;
        int max_frames = 0;
        int frames_shaking = 0;
        int speed = 0; 
        int phase_x = 0;
        int phase_y = 0;
    } thump, shake;

    void AFBGinit();
    void setTileOfAFBG(int tile, int index);
    void createAFBG();
    void updateMap();
    void startThumpAFBG(int amplitude_x, int amplitude_y, int frames);
    bool updateThumpAFBG();
    void startShakeAFBG(int amplitude_x, int amplitude_y, int speed);
    bool updateShakeAFBG();
    void endShakeAFBG();

    void rotScaleScrollAFBG(fixed32 angle, fixed32 scale_x, fixed32 scale_y, fixed32 scroll_x, fixed32 scroll_y);
    // void stopShakeAFBG(int speed);
    AffineBackground(int screen,
                     int layer,
                     const unsigned char tiles[],
                     int tile_size,
                     const unsigned short map[],
                     int map_size,
                     const unsigned short pal[],
                     int pal_size,
                     BgSize size);
};