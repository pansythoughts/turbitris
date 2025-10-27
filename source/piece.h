#pragma once

#include "enums.h"
#include "board.h"
#include "libs.h"
#include "globals.h"

const std::vector<std::vector<int>> shapes[] =
    {
        // NONE //
        {{0}},
        // 'O' //
        {
            {O, O},
            {O, O}},
        // 'I' //
        {
            {I},
            {I},
            {I},
            {I}},
        // 'L' //
        {
            {L, 0},
            {L, 0},
            {L, L}},
        // 'J' //
        {{0, J},
         {0, J},
         {J, J}},
        // 'S' //
        {
            {S, 0},
            {S, S},
            {0, S},
        },
        // 'Z' //
        {
            {0, Z},
            {Z, Z},
            {Z, 0},
        },
        // 'T' //
        {
            {0, T, 0},
            {T, T, T}}};

class Piece
{
public:
    int pos_x = 0;
    int pos_y = 0;
    int frames_moving = 0;
    int falling_frequency = 80;
    int frames_colliding_down = 0;
    int frames_since_hit_ground = 0;
    bool piece_solidified = false;

    bool hit_ground = false;

    Board *board = nullptr;

    PIECES shape = I;
    std::vector<std::vector<int>> current_piece_matrix;
    void printPiece();
    u8 checkCollision();
    void updatePiece();
    void movePiece();
    void hardDrop();
    void rotateCW();
    void rotateCCW();
    void initPiece();
    void solidifyPiece();
    void restartPiece();
    void pieceFall();
    void checkSolidify();
    void setPieceToGray();

    Piece(Board *board);
};