#include "game.h"

void Game::initGame()
{

    videoSetMode(MODE_4_2D);
    videoSetModeSub(MODE_4_2D);
    // i didn't wanna use nflib but i have no time :c.
    NF_InitSpriteBuffers();
    NF_InitSpriteSys(0);
    NF_InitSpriteSys(1);
    nfInit = nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

    // I HATE THIS BUT I HAVE NO TIME :C
    NF_LoadSpriteGfx("sprite/pause", 0, 64, 64);
    NF_LoadSpritePal("palette/pause", 0);
    NF_LoadSpriteGfx("sprite/continue", 1, 64, 64);
    NF_LoadSpritePal("palette/continue", 1);
    NF_LoadSpriteGfx("sprite/exit", 2, 64, 64);
    NF_LoadSpritePal("palette/exit", 2);
    NF_LoadSpriteGfx("sprite/selection_arrow", 3, 16, 16);
    NF_LoadSpritePal("palette/selection_arrow", 3);
    NF_LoadSpriteGfx("sprite/game_over", 4, 64, 64);
    NF_LoadSpritePal("palette/game_over", 4);
    NF_LoadSpriteGfx("sprite/retry", 5, 64, 64);
    NF_LoadSpritePal("palette/retry", 5);
    NF_LoadSpriteGfx("sprite/score", 6, 64, 64);
    NF_LoadSpritePal("palette/score", 6);
    NF_LoadSpriteGfx("sprite/numbers", 7, 32, 32);
    NF_LoadSpritePal("palette/numbers", 7);
    NF_LoadSpriteGfx("sprite/turbitris", 11, 64, 64);
    NF_LoadSpritePal("palette/turbitris", 11);
    NF_LoadSpriteGfx("sprite/by_pansythoughts", 13, 64, 64);
    NF_LoadSpritePal("palette/by_pansythoughts", 13);
    NF_LoadSpriteGfx("sprite/start", 14, 64, 64);
    NF_LoadSpritePal("palette/start", 14);
    NF_VramSpriteGfx(0, 0, 0, false);
    NF_VramSpritePal(0, 0, 0);
    NF_VramSpriteGfx(1, 1, 1, false);
    NF_VramSpritePal(1, 1, 1);
    NF_VramSpriteGfx(1, 2, 2, false);
    NF_VramSpritePal(1, 2, 2);
    NF_VramSpriteGfx(1, 3, 3, false);
    NF_VramSpritePal(1, 3, 3);
    NF_VramSpriteGfx(0, 4, 4, false);
    NF_VramSpritePal(0, 4, 4);
    NF_VramSpriteGfx(1, 5, 5, false);
    NF_VramSpritePal(1, 5, 5);
    NF_VramSpriteGfx(0, 6, 6, false);
    NF_VramSpritePal(0, 6, 6);
    NF_VramSpriteGfx(0, 7, 7, false);
    NF_VramSpritePal(0, 7, 7);
    NF_VramSpriteGfx(0, 11, 11, false);
    NF_VramSpritePal(0, 11, 11);
    NF_VramSpriteGfx(1, 13, 13, false);
    NF_VramSpritePal(1, 13, 13);
    NF_VramSpriteGfx(1, 14, 14, false);
    NF_VramSpritePal(1, 14, 14);

    if (debug)
        consoleDemoInit();
}

void Game::setupVRAMBanks()
{

    // table that describes the layout:
    // https://mtheall.com/banks.html#A=MBG1&B=MOBJ0&C=SBG&D=SOBJ&E=MBG&F=BGEPAL01&G=OBJEPAL&H=SBGEPAL&I=SOBJEPAL
    vramSetBankA(VRAM_A_MAIN_BG_0x06020000);
    vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
    vramSetBankD(VRAM_D_SUB_SPRITE);
    vramSetBankE(VRAM_E_MAIN_BG_0x06000000);
    vramSetBankF(VRAM_F_BG_EXT_PALETTE_SLOT01);
    vramSetBankG(VRAM_G_SPRITE_EXT_PALETTE);
    vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
    vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);
}

void Game::endMatch()
{
    if (current_match != nullptr)
    {
        delete current_match;
        current_match = nullptr;
    }
}

void Game::newMatch()
{
    if (current_match == nullptr)
        current_match = new Match();
}

void Game::consoleDebug()
{
    // CONSOLE DEBUG //
    consoleClear();
    /*
    std::cout << "Frames: " << frames
              << "\nPIECE POS_X: " << piece->pos_x
              << "\nPIECE POS_Y: " << piece->pos_y
              << "\n SHAKE AMPLITUDE X: " << board->blocks->thump.current_shake_amplitude_x
              << "\n SHAKE AMPLITUDE Y: " << board->blocks->thump.current_shake_amplitude_y
              << "\n SQRT 9: " << sqrt_(9)
              << "\n LINES CLEARED : ";
    */
    printf("Frames: %i", frames);
    printf("\nPIECE POS_X: %i", current_match->upper_piece->pos_x);
    printf("\nPIECE POS_Y: %i", current_match->upper_piece->pos_y);
    printf("\nSHAKE AMPLITUDE X: %i", current_match->upper_board->blocks->thump.current_shake_amplitude_x);
    printf("\nSHAKE AMPLITUDE Y: %i", current_match->upper_board->blocks->thump.current_shake_amplitude_y);
    printf("\nCOLLISION BITMASK: " BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(current_match->upper_piece->checkCollision()));
}

void Game::updateGame()
{

    scanInput();
    switch (game_state)
    {
    case MAIN_MENU:

        static int arrow_pos_x;
        static int arrow_pos_y;
        if (frames_state == 0)
        {
            NF_CreateSprite(0, 11, 11, 11, 70, 150);
            NF_CreateSprite(0, 12, 11, 11, 130, 150);
            NF_SpriteFrame(0, 12, 1);

            NF_CreateSprite(1, 13, 13, 13, 90, 150);

            NF_CreateSprite(1, 14, 14, 14, 90, 0);

            NF_CreateSprite(1, 3, 3, 3, 120, 37);
            NF_EnableSpriteRotScale(1, 3, 3, true);
            NF_SpriteRotScale(1, 3, 0, 380, 380);
        }

        arrow_pos_x = 105;

        if (frames_state % 20 == 0)
            arrow_pos_y = (arrow_pos_y == 38) ? 50 : 38;

        NF_MoveSprite(1, 3, arrow_pos_x, arrow_pos_y);

        if (keys_down & KEY_A || keys_down & KEY_START || keys_down & KEY_TOUCH)
        {
            NF_DeleteSprite(0, 11);
            NF_DeleteSprite(0, 12);
            NF_DeleteSprite(1, 13);
            NF_DeleteSprite(1, 14);
            NF_DeleteSprite(1, 3);
            game_state = MATCH;
            frames_state = -1;
        }

        break;

    case MATCH:
        if (frames_state == 0)
        {
            if (current_match)
            {
                current_match->match_needs_reset = false;
                current_match->match_exit_game_over = false;
                current_match->match_exit_pause = false;
            }
            newMatch();
        }

        if (current_match->match_needs_reset)
        {
            current_match->exitGameOver();
            current_match->resetMatch();
            current_match->match_needs_reset = false;
        }

        if (current_match->match_exit_game_over)
        {
            game_state = MAIN_MENU;
            current_match->current_playing_piece->shape = NONE;
            current_match->current_standby_piece->shape = NONE;
            current_match->exitGameOver();
            current_match->resetMatch();
            current_match->match_exit_game_over = false;
            frames_state = -1;
        }

        if (current_match->match_exit_pause)
        {
            game_state = MAIN_MENU;
            current_match->current_playing_piece->shape = NONE;
            current_match->current_standby_piece->shape = NONE;
            current_match->exitPauseMenu();
            current_match->resetMatch();
            current_match->match_exit_game_over = false;
            frames_state = -1;
        }

        current_match->updateMatch();
        break;
    }

    if (debug)
        consoleDebug();

    NF_SpriteOamSet(0);
    NF_SpriteOamSet(1);
    swiWaitForVBlank();
    oamUpdate(&oamMain);
    oamUpdate(&oamSub);
    bgUpdate();

    frames_state++;
}