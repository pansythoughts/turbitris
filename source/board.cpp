#include "board.h"

void Board::initBoard(int screen)
{
    board_matrix = std::vector<std::vector<int>>(board_height, std::vector<int>(board_width));

    for (int i = 0; i < board_height; i++)
    {
        for (int j = 0; j < board_width; j++)
        {
            board_matrix[i][j] = 0; //(i % 6) + 1; <- RAINBOW
        }
    }

    blocks = new AffineBackground(screen,
                                  2,
                                  blocksTiles,
                                  blocksTilesLen,
                                  nullptr,
                                  0,
                                  blocksPal,
                                  blocksPalLen,
                                  BgSize_R_256x256);
    blocks->createAFBG();
    blocks->rotScaleScrollAFBG(0, 255, 255, inttof32(-board_pos_x * 8), inttof32(board_pos_y));
}

void Board::printBoard()
{
    for (int i = 0; i < board_height; i++)
    {
        for (int j = 0; j < board_width; j++)
        {
            int tile = ((j) + (i * (SCREEN_WIDTH / 8)));
            blocks->map_buffer[tile] = board_matrix[i][j];
        }
    }
    blocks->updateMap();
}

void Board::updateBoard()
{
    printBoard();
    blocks->updateThumpAFBG();
}

int Board::leftTileLimit()
{
    return 0;
}

int Board::rightTileLimit()
{
    return board_width;
}

int Board::bottomTileLimit()
{
    return board_height;
}

int Board::topTileLimit()
{
    return 0;
}

int Board::boardCenter()
{
    return (board_width / 2);
}

void Board::removeLines()
{
    std::vector<std::vector<int>> new_matrix;
    std::vector<int> empty_row(board_width, 0);
    for (int i = 0; i < board_height; i++)
    {
        bool current_row_full = true;
        for (int j = 0; j < board_width; j++)
        {
            if (board_matrix[i][j] == 0)
            {
                current_row_full = false;
                break;
            }
        }
        if (!current_row_full)
            new_matrix.push_back(board_matrix[i]);
    }

    int lines_cleared = board_height - new_matrix.size();
    bool matrix_changed = lines_cleared > 0;

    if (matrix_changed)
    {
        for (int i = 0; i < lines_cleared; i++)
            new_matrix.insert(new_matrix.begin(), empty_row);

        board_matrix = new_matrix;
    }
    this->cleared_lines = lines_cleared;
}

void Board::resetBoard()
{
    for (int i = 0; i < board_height; i++)
    {
        for (int j = 0; j < board_width; j++)
        {
            board_matrix[i][j] = 0;
        }
    }
}

void Board::setBoardTilesToGray()
{
    for (int i = 0; i < board_height; i++)
    {
        for (int j = 0; j < board_width; j++)
        {
            int tile = ((j) + (i * (SCREEN_WIDTH / 8)));
            if (board_matrix[i][j] != NONE)
                blocks->map_buffer[tile] = GRAY;
            else
                blocks->map_buffer[tile] = NONE;
        }
    }
    blocks->updateMap();
}

Board::Board(int screen)
{
    initBoard(screen);
}