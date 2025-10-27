#include "affinebackground.h"

void AffineBackground::createAFBG()
{
    dmaCopy(tiles, bgGetGfxPtr(id), tile_size);
    if (screen == 0)
        dmaCopy(pal, BG_PALETTE, pal_size);
    else
        dmaCopy(pal, BG_PALETTE_SUB, pal_size);

    if (map != nullptr)
        dmaCopy(map, bgGetMapPtr(id), map_size);
}

void AffineBackground::AFBGinit()
{

    // first 16kb reserved for affine and tiled bases
    // (this means first base for BMP mapbase or tile bases,
    // first 8 bases for affine and tiled mapbases)
    for (int i = 0; i < 8; i++)
    {
        if (regular_map_bases_screen_1[i].available)
        {
            map_base = i;
            regular_map_bases_screen_1[i].available = false;
            break;
        }
    }
    for (int i = 1; i < 32; i++)
    {
        if (BMP_and_tile_bases_screen_1[i].available)
        {
            tile_base = i;
            BMP_and_tile_bases_screen_1[i].available = false;
            break;
        }
    }
    if (screen == 0)
        id = bgInit(layer, BgType_Rotation, size, map_base, tile_base);
    else
        id = bgInitSub(layer, BgType_Rotation, size, map_base, tile_base);
}

void AffineBackground::setTileOfAFBG(int tile, int index)
{
    volatile u8 *mapptr = (volatile u8 *)bgGetMapPtr(id);
    mapptr[tile] = (volatile int)index;
}

void AffineBackground::updateMap()
{
    mapptr = (volatile u8 *)bgGetMapPtr(id);
    dmaCopy((void *)map_buffer, (void *)mapptr, sizeof(map_buffer));
}

// call this once every time a new thump effect is wanted.
void AffineBackground::startThumpAFBG(int amplitude_x, int amplitude_y, int frames)
{
    thump.shaking = true;
    thump.target_shake_amplitude_x = amplitude_x;
    thump.target_shake_amplitude_y = amplitude_y;
    thump.max_frames = frames;
}

// call this repeatedly to thump for the amount of frames given.
// it will stop after that amount of frames, regardless if the function is being called.
// amplitude = max pixels you want it to move.
// returns whether a thump ocurred during the frame or not.
bool AffineBackground::updateThumpAFBG()
{

    /*\
    // drug effect
    scale_x += scale_direction;

    if (scale_x >= 512 || scale_x < 0)
        scale_direction *= -1;
    bgSetRotateScale(id, scale_x, scale_x, scale_y);

    scroll_x = (-1 * 8) + (scale_x / 50);
    scroll_y = (scale_x / 50);
    */

    if ((thump.target_shake_amplitude_x == 0 && thump.target_shake_amplitude_y == 0) || (thump.frames_shaking >= thump.max_frames) || !thump.shaking)
    {
        thump.current_shake_amplitude_x = 0;
        thump.current_shake_amplitude_y = 0;
        thump.shaking = false;
        thump.frames_shaking = 0;
        return false;
    }
    fixed32 t = divf32(inttof32(thump.frames_shaking), inttof32(thump.max_frames));
    fixed32 scroll = (((floattof32(3.5f) * sqrtf32(t)) >> 12) * ((inttof32(1) - t) * (inttof32(1) - t) >> 12) >> 12); // sqrt-quadratic. this is kind of unreadable but it just fixed-point multiplies all of the terms.
    // 2.6f * (float)sqrt_(t) * (float)(1.0f - t); // sqrt-linear
    // 4.2f * (float)sqrt_(t) * (float)(1.0f - t) * (float)(1.0f - t) * (float)(1.0f - t); // sqrt-cubic
    // other options. first one not as smooth tho. second one should be smoother, at the cost of more power.

    thump.current_shake_amplitude_x = thump.target_shake_amplitude_x * scroll;
    thump.current_shake_amplitude_y = thump.target_shake_amplitude_y * scroll;

    bgSetScrollf(id, (scroll_x + thump.current_shake_amplitude_x) >> 4, (scroll_y + thump.current_shake_amplitude_y) >> 4); // >> 4 shift because .12 -> .8 (?)
    thump.frames_shaking++;
    return true;

    // shitty implementation //
    /*
    if ((amplitude_x == 0 && amplitude_y == 0) || shake_data.frames_shaking >= frames)
    {
        return;
    }

    shake_data.speed_x += (shake_data.acc_x * amplitude_x);
    shake_data.speed_y += (shake_data.acc_y * amplitude_y);

    shake_data.current_shake_amplitude_x += ((amplitude_x > 0) ? shake_data.speed_x : -shake_data.speed_x);
    shake_data.current_shake_amplitude_y += ((amplitude_y > 0) ? shake_data.speed_y : -shake_data.speed_y);

    if (std::abs(shake_data.current_shake_amplitude_x) < std::abs(amplitude_x) && shake_data.frames_shaking <= (frames / 2))
    {
        shake_data.acc_x += ((amplitude_x > 0) ? (1 / frames) : -(1 / frames));
    }
    else
    {
        shake_data.acc_x += ((amplitude_x > 0) ? -(1 / frames) : (1 / frames));
    }

    if (std::abs(shake_data.current_shake_amplitude_y) < std::abs(amplitude_y) && shake_data.frames_shaking <= (frames / 2))
    {
        shake_data.acc_y += ((amplitude_y > 0) ? -(0.0001f * frames) : (0.0001f * frames));
    }
    else
    {
        shake_data.acc_y += ((amplitude_y > 0) ? (0.0001f * frames) : -(0.0001f * frames));
    }

    bgSetScroll(id, shake_data.current_shake_amplitude_x, shake_data.current_shake_amplitude_y);
    shake_data.frames_shaking++;
    */
}

void AffineBackground::rotScaleScrollAFBG(fixed32 angle, fixed32 scale_x, fixed32 scale_y, fixed32 scroll_x, fixed32 scroll_y)
{
    // bgSetRotateScale(id, angle, scale_x, scale_y);
    // bgSetScroll(id, scroll_x, scroll_y);

    bgSet(id, angle, scale_x, scale_y, scroll_x, scroll_y, 0, 0);
    bgSetScrollf(id, scroll_x >> 4, scroll_y >> 4); // >> 4 shift because .12 -> .8 (?)

    this->angle = angle;
    this->scale_x = scale_x;
    this->scale_y = scale_y;
    this->scroll_x = scroll_x;
    this->scroll_y = scroll_y;
}

void AffineBackground::startShakeAFBG(int amplitude_x, int amplitude_y, int speed)
{
    shake.shaking = true;
    shake.target_shake_amplitude_x = amplitude_x;
    shake.target_shake_amplitude_y = amplitude_y;
    shake.speed = speed;
}

bool AffineBackground::updateShakeAFBG()
{
    if ((shake.target_shake_amplitude_x == 0 && shake.target_shake_amplitude_y == 0))
    {
        shake.current_shake_amplitude_x = 0;
        shake.current_shake_amplitude_y = 0;
        shake.shaking = false;
        shake.frames_shaking = 0;
        return false;
    }

    shake.phase_x = rand() % 4;
    shake.phase_y = rand() % 4;
    fixed32 tx = (sinLerp(((shake.frames_shaking % 32768) - (1 << 15) + shake.phase_x) * shake.speed));
    fixed32 ty = (cosLerp((shake.frames_shaking % 32768 + shake.phase_y) * shake.speed - 8));

    shake.current_shake_amplitude_x = mulf32(tx, shake.target_shake_amplitude_x);
    shake.current_shake_amplitude_y = mulf32(ty, shake.target_shake_amplitude_y);

    bgSetScrollf(id, (scroll_x + thump.current_shake_amplitude_x + shake.current_shake_amplitude_x) >> 4, (scroll_y + thump.current_shake_amplitude_y + shake.current_shake_amplitude_y - 10000) >> 4); // >> 4 shift because .12 -> .8 (?)
    shake.frames_shaking++;
    return true;
}

AffineBackground::AffineBackground(int screen,
                                   int layer,
                                   const unsigned char tiles[],
                                   int tile_size,
                                   const unsigned short map[],
                                   int map_size,
                                   const unsigned short pal[],
                                   int pal_size,
                                   BgSize size)
{

    this->screen = screen;
    this->layer = layer;
    this->tiles = tiles;
    this->map = map;
    this->pal = pal;
    this->tile_size = tile_size;
    this->map_size = map_size;
    this->pal_size = pal_size;
    this->size = size;
    AFBGinit();
}