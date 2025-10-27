#pragma once

#include "libs.h"
#include "enums.h"
#include "affinebackground.h"
#include "globals.h"
#include "input.h"

#include "gfx/blocks.h"

class Board
{
private:
public:
    std::vector<std::vector<int>> board_matrix;
    AffineBackground *blocks = nullptr;

    int board_width = BOARD_DEFAULT_WIDTH;
    int board_height = BOARD_DEFAULT_HEIGHT;
    int board_pos_x = 10;
    int board_pos_y = 0;
    int cleared_lines = 0;

    void initBoard(int screen);
    void printBoard();
    void updateBoard();
    int leftTileLimit();
    int rightTileLimit();
    int bottomTileLimit();
    int topTileLimit();
    int boardCenter();
    void removeLines();
    void resetBoard();
    void setBoardTilesToGray();

    Board(int screen);
};