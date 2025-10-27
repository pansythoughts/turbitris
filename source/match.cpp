#include "match.h"

void Match::initMatch()
{

    upper_board = new Board(0);
    lower_board = new Board(1);
    upper_piece = new Piece(upper_board);
    lower_piece = new Piece(lower_board);

    // by default.
    switchPlayingBoard();

    current_playing_board->blocks->startShakeAFBG(1 << 13, 1 << 13, 64);
    current_standby_board->blocks->startShakeAFBG(1 << 13, 1 << 13, 64);
}

void Match::endMatch()
{
    delete upper_board;
    delete lower_board;
    delete upper_piece;
    delete lower_piece;
}

void Match::updateMatch()
{
    if (match_game_over)
    {
        gameOver();
        return;
    }
    if (!match_paused)
        exitPauseMenu();

    if (keys_down & KEY_START)
        match_paused = !match_paused;

    if (match_paused)
    {
        showPauseMenu();
        frames_paused++;
        return;
    }

    current_playing_board->updateBoard();
    current_playing_board->blocks->updateShakeAFBG();
    current_standby_board->setBoardTilesToGray();
    current_standby_board->blocks->updateShakeAFBG();

    current_playing_piece->updatePiece();
    current_standby_piece->setPieceToGray();
    current_standby_piece->pieceFall();
    current_standby_piece->checkSolidify();

    if (current_playing_piece->checkCollision() & COLLISION_DIRECTION::UP || current_standby_piece->checkCollision() & COLLISION_DIRECTION::UP)
    {
        current_playing_board->setBoardTilesToGray();
        current_playing_board->resetBoard();
        current_standby_board->resetBoard();
        current_playing_piece->restartPiece();
        match_game_over = true;
    }

    if (current_playing_board->cleared_lines > 0)
    {
        score += current_playing_board->cleared_lines * 10;
        current_playing_board->cleared_lines = 0;
    }

    if (current_playing_piece->piece_solidified || current_standby_piece->piece_solidified)
    {
        score++;
        current_playing_piece->piece_solidified = false;
        current_standby_piece->piece_solidified = false;
    }
    manageTurboAndSlowdown();

    if (keys_down & KEY_L)
        next_board_switch = true;

    if (next_board_switch)
    {
        switchPlayingBoard();
        next_board_switch = false;
    }
}

void Match::manageTurboAndSlowdown()
{
    if (!turbo_on)
    {
        if (time_on_turbo > 0)
        {
            upper_piece->falling_frequency = default_fall_frequency;
            lower_piece->falling_frequency = default_fall_frequency;
            time_on_turbo = 0;
            upper_board->blocks->startShakeAFBG(1 << 13, 1 << 13, 64);
            lower_board->blocks->startShakeAFBG(1 << 13, 1 << 13, 64);
        }
        time_since_last_turbo++;
    }
    else
    {
        time_on_turbo++;
        time_since_last_turbo = 0;

        if (time_on_turbo == 1)
            if (rand() % 21 <= 10)
            {
                upper_piece->falling_frequency = 2;
                upper_board->blocks->startShakeAFBG(1 << 13, 1 << 13, 512);
                lower_piece->falling_frequency = 100;
            }
            else
            {
                lower_piece->falling_frequency = 2;
                lower_board->blocks->startShakeAFBG(1 << 13, 1 << 13, 512);
                upper_piece->falling_frequency = 100;
            }
    }

    if (time_since_last_turbo >= 15 * 60)
    {
        if (rand() % 301 == 300)
            turbo_on = true;
    }

    if (time_on_turbo >= 10 * 60)
        turbo_on = false;
}

void Match::switchPlayingBoard()
{
    if (current_playing_board == upper_board)
    {
        current_playing_board = lower_board;
        current_playing_piece = lower_piece;
        current_standby_board = upper_board;
        current_standby_piece = upper_piece;
    }
    else
    {
        current_playing_board = upper_board;
        current_playing_piece = upper_piece;
        current_standby_board = lower_board;
        current_standby_piece = lower_piece;
    }
}

void Match::showPauseMenu()
{
    static int arrow_pos_x;
    static int arrow_pos_y;

    if (frames_paused == 0)
    {
        NF_CreateSprite(0, 0, 0, 0, 95, 150);
        NF_CreateSprite(1, 1, 1, 1, 61, 0);
        NF_CreateSprite(1, 2, 2, 2, 135, 0);
        NF_CreateSprite(1, 3, 3, 3, 78, 37);
        NF_EnableSpriteRotScale(1, 3, 3, true);
        NF_SpriteRotScale(1, 3, 0, 380, 380);
        current_playing_board->setBoardTilesToGray();
        current_playing_piece->setPieceToGray();
    }

    if (keys_down & KEY_RIGHT || keys_down & KEY_LEFT)
        if (current_menu_option == 0)
            current_menu_option = 1;
        else
            current_menu_option = 0;

    arrow_pos_x = 77 + (current_menu_option * 74);

    if (frames_paused % 20 == 0)
        arrow_pos_y = (arrow_pos_y == 38) ? 50 : 38;

    NF_MoveSprite(1, 3, arrow_pos_x, arrow_pos_y);

    if (keys_down & KEY_A)
        if (current_menu_option == 0)
            match_paused = false;
        else
        {
            match_exit_pause = true;
            match_paused = false;
        }

    // NF_MoveSprite(1, 3, 83, 45 + ((sinLerp(frames_paused * 400) * 10) >> 12));
}

void Match::exitPauseMenu()
{
    if (NF_SPRITEOAM[0][0].created)
        NF_DeleteSprite(0, 0);

    for (int i = 1; i <= 3; i++)
    {
        if (NF_SPRITEOAM[1][i].created)
            NF_DeleteSprite(1, i);
    }
    frames_paused = 0;
}

void Match::gameOver()
{
    static int arrow_pos_x;
    static int arrow_pos_y;
    if (frames_game_over == 0)
    {
        NF_CreateSprite(0, 4, 4, 4, 95, 100);
        NF_CreateSprite(1, 5, 5, 5, 61, 0);
        NF_CreateSprite(1, 2, 2, 2, 135, 0);
        NF_CreateSprite(1, 3, 3, 3, 78, 37);
        NF_EnableSpriteRotScale(1, 3, 3, true);
        NF_SpriteRotScale(1, 3, 0, 380, 380);
        showScore();
    }

    if (keys_down & KEY_RIGHT || keys_down & KEY_LEFT)
        if (current_menu_option == 0)
            current_menu_option = 1;
        else
            current_menu_option = 0;

    arrow_pos_x = 77 + (current_menu_option * 74);

    if (frames_game_over % 20 == 0)
        arrow_pos_y = (arrow_pos_y == 38) ? 50 : 38;

    NF_MoveSprite(1, 3, arrow_pos_x, arrow_pos_y);

    if (keys_down & KEY_A)
        if (current_menu_option == 0)
        {
            match_game_over = false;
            match_needs_reset = true;
        }
        else
        {
            match_game_over = false;
            match_exit_game_over = true;
        }

    frames_game_over++;
}

void Match::exitGameOver()
{
    for (int i = 2; i <= 10; i++)
        if (i == 4 || i >= 6)
            NF_DeleteSprite(0, i);
        else
            NF_DeleteSprite(1, i);
}

void Match::showScore()
{
    int thousands, hundreds, tens, ones;

    thousands = (score / 1000) % 10;
    hundreds = (score / 100) % 10;
    tens = (score / 10) % 10;
    ones = score % 10;

    if (thousands > 9)
    {
        thousands = 9;
        hundreds = 9;
        tens = 9;
        ones = 9;
    }

    // show score text.

    NF_CreateSprite(0, 6, 6, 6, 95, 0);

    for (int i = 0; i < 4; i++)
    {
        NF_CreateSprite(0, 7 + i, 7, 7, 98 + (i * 16), 32);
    }

    NF_SpriteFrame(0, 7, thousands);
    NF_SpriteFrame(0, 8, hundreds);
    NF_SpriteFrame(0, 9, tens);
    NF_SpriteFrame(0, 10, ones);
}

void Match::resetMatch()
{
    current_playing_board->resetBoard();
    current_playing_piece->restartPiece();

    current_standby_board->resetBoard();
    current_standby_piece->restartPiece();

    turbo_on = false;
    time_since_last_turbo = 0;
    time_on_turbo = 0;
    match_paused = false;
    match_game_over = false;
    frames_paused = 0;
    frames_game_over = 0;
    current_menu_option = 0;
    default_fall_frequency = 80;

    score = 0;
}

Match::Match()
{
    initMatch();
}

Match::~Match()
{
    endMatch();
}