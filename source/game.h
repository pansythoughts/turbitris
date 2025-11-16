#pragma once

#include "libs.h"

#include "enums.h"
#include "input.h"
#include "board.h"
#include "piece.h"
#include "globals.h"
#include "match.h"
#include "bitmapbackground.h"

class Game
{
private:
public:
    bool nfInit = false;
    bool fatInit = false;
    bool debug = false;
    int frames_state = 0;

    GAME_STATES game_state = MAIN_MENU;

    Match *current_match = nullptr;
    BitmapBackground *custom_image = nullptr;
    void initGame();
    void updateGame();
    void newMatch();
    void endMatch();
    void setupVRAMBanks();
    void consoleDebug();
};