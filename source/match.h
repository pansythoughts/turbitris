#pragma once

#include "libs.h"
#include "board.h"
#include "piece.h"

class Match
{
public:
    Board *upper_board = nullptr;
    Board *lower_board = nullptr;

    Piece *upper_piece = nullptr;
    Piece *lower_piece = nullptr;

    Board *current_playing_board = nullptr;
    Piece *current_playing_piece = nullptr;

    Board *current_standby_board = nullptr;
    Piece *current_standby_piece = nullptr;

    bool turbo_on = false;
    int time_since_last_turbo = 0;
    int time_on_turbo = 0;
    bool match_paused = false;
    bool match_game_over = false;
    int frames_paused = 0;
    int frames_game_over = 0;
    int current_menu_option = 0;
    int default_fall_frequency = 80;
    bool match_needs_reset = false;
    bool match_exit_game_over = false;
    bool match_exit_pause = false;

    bool next_board_switch = false;

    int score = 0;

    void initMatch();
    void endMatch();
    void updateMatch();
    void manageTurboAndSlowdown();
    void switchPlayingBoard();
    void showPauseMenu();
    void exitPauseMenu();
    void gameOver();
    void exitGameOver();
    void showScore();
    void resetMatch();
    Match(); // this constructor could maybe take as parameters the settings of the match (game mode, for example).
    ~Match();
};