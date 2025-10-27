#pragma once
// all enums and #defines.

#define sqrt_(n) (fixedToFloat(sqrtf32(floatToFixed((float)n, 16)), 16) * 4.0f)
#define fixed32 s32

// to print u8 in binary format
// (thanks stackoverflow)
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)         \
    ((byte) & 0x80 ? '1' : '0'),     \
        ((byte) & 0x40 ? '1' : '0'), \
        ((byte) & 0x20 ? '1' : '0'), \
        ((byte) & 0x10 ? '1' : '0'), \
        ((byte) & 0x08 ? '1' : '0'), \
        ((byte) & 0x04 ? '1' : '0'), \
        ((byte) & 0x02 ? '1' : '0'), \
        ((byte) & 0x01 ? '1' : '0')

#define BOARD_DEFAULT_WIDTH 10
#define BOARD_DEFAULT_HEIGHT 20

enum PIECES
{
    NONE,
    O,
    I,
    L,
    J,
    S,
    Z,
    T,
    GRAY
};

// u8 bitmask
enum COLLISION_DIRECTION
{
    LEFT = (1 << 0),  // 00000001
    RIGHT = (1 << 1), // 00000010
    UP = (1 << 2),    // 00000100
    DOWN = (1 << 3)   // 00001000
};

enum GAME_STATES
{
    MAIN_MENU, MATCH
};