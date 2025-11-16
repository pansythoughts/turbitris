#include "piece.h"

void Piece::initPiece()
{
    srand(time(0) + frames + pos_x + pos_y);
    shape = (PIECES)((rand() % 7) + 1);
    current_piece_matrix = shapes[shape];

    current_piece_matrix = shapes[shape];

    pos_x = board->boardCenter();
    pos_y = 0;
}

void Piece::updatePiece()
{
    printPiece();
    movePiece();
}

void Piece::movePiece()
{
    int temp_pos_x = pos_x;
    int temp_pos_y = pos_y;

    if (keys_down & KEY_UP)
    {
        hardDrop();
    }

    pieceFall();

    if (frames_moving == 1 || frames_moving > 5)
    {
        if (keys_held & KEY_LEFT && !(checkCollision() & COLLISION_DIRECTION::LEFT) && (frames_moving % 2 == 0 || frames_moving == 1))
        {
            temp_pos_x--;
            if (temp_pos_x >= board->leftTileLimit() && temp_pos_x < board->rightTileLimit())
            {
                pos_x = temp_pos_x;
            }
        }

        if (keys_held & KEY_RIGHT && !(checkCollision() & COLLISION_DIRECTION::RIGHT) && (frames_moving % 2 == 0 || frames_moving == 1))
        {
            temp_pos_x++;
            if (temp_pos_x >= board->leftTileLimit() && temp_pos_x < board->rightTileLimit())
            {
                pos_x = temp_pos_x;
            }
        }
    }
    checkSolidify();
    if (keys_held & (KEY_LEFT | KEY_RIGHT | KEY_DOWN))
        frames_moving++;
    else
        frames_moving = 0;

    if (keys_down & KEY_B)
        rotateCW();

    if (keys_down & KEY_A)
        rotateCCW();
}

void Piece::hardDrop()
{
    /* shitty and over-engineered as fuck approach
    std::vector<int> max_pos_y;
    for (int i = pos_y; i < board->board_height; i++)
    {
        for (int j = pos_x; j < pos_x + (int)current_piece_matrix[0].size(); j++)
        {
            if (current_piece_matrix[i - pos_y][j - pos_x] == 0)
                continue;
            if (i < board->board_height && j < board->board_width)
            {
                if (board->board_matrix[i][j] != 0)
                {
                    max_pos_y.push_back(i - current_piece_matrix.size());
                }
            }
        }
        if (!max_pos_y.empty())
        {
            pos_y = *std::max_element(max_pos_y.begin(), max_pos_y.end());
            return;
        }
    }
        */

    while (!(checkCollision() & COLLISION_DIRECTION::DOWN))
    {
        pos_y++;
    }
    frames_colliding_down = 60;
}
void Piece::printPiece()
{
    for (int i = 0; i < (int)current_piece_matrix.size(); i++)
    {
        for (int j = 0; j < (int)current_piece_matrix[0].size(); j++)
        {
            int tile = ((j + pos_x) + ((i + pos_y) * (SCREEN_WIDTH / 8))); // 8 pixels per tile.
            if (current_piece_matrix[i][j] != NONE)
                board->blocks->map_buffer[tile] = current_piece_matrix[i][j];
        }
    }
    board->blocks->updateMap();
}

/*
   OKAY. so to make this right, i'd have to implement the system that
   is actually used, where it would check all of the possible rotation
   situations, and check if it matches.
   but that would require a lot of effort so, for now,
   i'm sticking with what's below
   can get glitchy, but works for the most part.
*/
void Piece::rotateCCW()
{
    if (shape == O || (checkCollision() & RIGHT && checkCollision() & LEFT && pos_x >= board->rightTileLimit() - 2) || (checkCollision() & UP && checkCollision() & DOWN && checkCollision() & RIGHT && checkCollision() & LEFT))
        return;

    std::vector<std::vector<int>> rotated_piece;

    std::vector<std::vector<int>> unrotated_piece = current_piece_matrix;

    for (int j = int(current_piece_matrix[0].size()) - 1; j >= 0; j--)
    {
        std::vector<int> row;
        for (int i = 0; i < int(current_piece_matrix.size()); i++)
        {
            row.push_back(current_piece_matrix[i][j]);
        }
        rotated_piece.push_back(row);
    }
    current_piece_matrix = rotated_piece;

    // i hate everything that is below, but it must stick for now.
    while (checkCollision() & COLLISION_DIRECTION::RIGHT && !(checkCollision() & COLLISION_DIRECTION::LEFT))
    {
        pos_x--;
    }
    pos_x++;
    if (!(checkCollision() & COLLISION_DIRECTION::RIGHT))
    {
        pos_x--;
    }

    while (checkCollision() & COLLISION_DIRECTION::DOWN && !(checkCollision() & COLLISION_DIRECTION::UP) && pos_y > board->topTileLimit())
    {
        pos_y--;
    }
    while (checkCollision() & COLLISION_DIRECTION::LEFT && !(checkCollision() & COLLISION_DIRECTION::RIGHT) && pos_x < board->rightTileLimit() - 1)
    {
        pos_x++;
    }
    while (checkCollision() & COLLISION_DIRECTION::RIGHT && !(checkCollision() & COLLISION_DIRECTION::LEFT) && pos_x > board->leftTileLimit())
    {
        pos_x--;
    }
}

void Piece::rotateCW()
{
    // lol. lazy af but works.
    for (int i = 0; i < 3; i++)
        rotateCCW();
}

uint8 Piece::checkCollision()
{
    uint8 collision_direction = 0;

    for (int i = 0; i < (int)current_piece_matrix.size(); i++)
    {
        for (int j = 0; j < (int)current_piece_matrix[0].size(); j++)
        {
            int pos_in_board_y = i + pos_y;
            int pos_in_board_x = j + pos_x;
            if (pos_in_board_y < board->board_height && pos_in_board_x < board->board_width)
            {
                if (pos_in_board_y + 1 < board->board_height)
                {
                    if (board->board_matrix[pos_in_board_y + 1][pos_in_board_x] != 0 && current_piece_matrix[i][j] != 0)
                        collision_direction |= COLLISION_DIRECTION::DOWN;
                }

                if (pos_in_board_y - 1 >= 0 && pos_in_board_y == 1) // don't really like this last condition but works for now.
                {
                    if (board->board_matrix[pos_in_board_y - 1][pos_in_board_x] != 0 && current_piece_matrix[i][j] != 0)
                        collision_direction |= COLLISION_DIRECTION::UP;
                }

                if (pos_in_board_x - 1 >= 0)
                {
                    if (board->board_matrix[pos_in_board_y][pos_in_board_x - 1] != 0 && current_piece_matrix[i][j] != 0)
                        collision_direction |= COLLISION_DIRECTION::LEFT;
                }
                if (pos_in_board_x + 1 < board->board_width)
                {
                    if (board->board_matrix[pos_in_board_y][pos_in_board_x + 1] != 0 && current_piece_matrix[i][j] != 0)
                        collision_direction |= COLLISION_DIRECTION::RIGHT;
                }
            }
        }
    }

    // --  board limits collision detections  -- //
    if (pos_x + (int)current_piece_matrix[0].size() >= board->rightTileLimit())
        collision_direction |= COLLISION_DIRECTION::RIGHT;

    if (pos_x <= board->leftTileLimit())
        collision_direction |= COLLISION_DIRECTION::LEFT;

    // useless: pos_y can be less or  equal than that

    // if (pos_y <= board->topTileLimit() - 1)
    // collision_direction |= COLLISION_DIRECTION::UP;

    if (pos_y + (int)current_piece_matrix.size() >= board->bottomTileLimit())
        collision_direction |= COLLISION_DIRECTION::DOWN;

    return collision_direction;
}

void Piece::solidifyPiece()
{
    for (int i = 0; i < (int)current_piece_matrix.size(); i++)
    {
        for (int j = 0; j < (int)current_piece_matrix[0].size(); j++)
        {
            if (current_piece_matrix[i][j] == 0)
                continue;

            if (i + pos_y < board->board_height && i + pos_y >= 0 && j + pos_x < board->board_width && j + pos_x >= 0)
                board->board_matrix[i + pos_y][j + pos_x] = current_piece_matrix[i][j];
        }
    }
    board->removeLines();
    if (board->cleared_lines == 0 && (keys_down & KEY_UP))
        board->blocks->startThumpAFBG(0, -5, 10);
    else
        board->blocks->startThumpAFBG(0, -(5 * board->cleared_lines), 20);

    restartPiece();
    piece_solidified = true;
}

void Piece::restartPiece()
{
    pos_y = 0;
    hit_ground = false;
    pos_x = board->boardCenter();
    srand(time(0) + frames + pos_x + pos_y);

    shape = (PIECES)((rand() % 7) + 1);
    current_piece_matrix = shapes[shape];
}

void Piece::pieceFall()
{
    int temp_pos_x = pos_x;
    int temp_pos_y = pos_y;

    if (frames % falling_frequency == 0 && frames > 0 && !(checkCollision() & COLLISION_DIRECTION::DOWN))
    {
        temp_pos_y++;
        if (temp_pos_y > board->topTileLimit() && temp_pos_y < board->bottomTileLimit())
        {
            pos_y = temp_pos_y;
        }
    }

    if (frames_moving == 1 || frames_moving > 5)
    {

        if (keys_held & KEY_DOWN && !(checkCollision() & COLLISION_DIRECTION::DOWN) && (frames_moving % 2 == 0 || frames_moving == 1) && (frames % falling_frequency != 0))
        {
            temp_pos_y++;
            if (temp_pos_y > board->topTileLimit() && temp_pos_y < board->bottomTileLimit())
            {
                pos_y = temp_pos_y;
            }
        }
    }
}

void Piece::checkSolidify()
{
    if (checkCollision() & COLLISION_DIRECTION::DOWN)
    {
        hit_ground = true;
        frames_colliding_down++;
    }
    else
    {
        frames_colliding_down = 0;
    }

    if (hit_ground)
        frames_since_hit_ground++;
    else
        frames_since_hit_ground = 0;

    if ((checkCollision() & COLLISION_DIRECTION::DOWN && (frames_colliding_down >= 60 || frames_since_hit_ground >= 240)))
    {
        solidifyPiece();
        frames_colliding_down = 0;
    }
}

void Piece::setPieceToGray()
{
    for (int i = 0; i < (int)current_piece_matrix.size(); i++)
    {
        for (int j = 0; j < (int)current_piece_matrix[0].size(); j++)
        {
            int tile = ((j + pos_x) + ((i + pos_y) * (SCREEN_WIDTH / 8))); // 8 pixels per tile.
            if (current_piece_matrix[i][j] != NONE)
                board->blocks->map_buffer[tile] = GRAY;
        }
    }
    board->blocks->updateMap();
}

Piece::Piece(Board *board)
{
    this->board = board;
    initPiece();
}