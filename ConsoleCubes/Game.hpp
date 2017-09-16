#pragma once

#include "Console.hpp"
#include "Field.hpp"
#include "Block.hpp"
#include "Timer.hpp"
#include "Menu.hpp"
#include "ParameterMenuOption.hpp"


enum class GameMode
{
    GAME = 1,
    MENU,
};

class Game
{
    Console mConsole;
    GameMode mCurrentMode;
    Field mField;
    Field mNextBlockField;
    Timer mGameTimer;

    uint32_t mFieldOffsetX;
    uint32_t mFieldOffsetY;

    bool mMainLoopActive;
    bool mNeedsRedraw;
    bool mCleanRowAnim;

    Block* mCurrentBlock;
    Block* mNextBlock;
    uint32_t mNextBlockInd;
    double mBlockFallTime;

    uint32_t mClearedRows[4];
    uint32_t mClearedRowsCount;
    uint32_t mAnimationStep;
    double mAnimationCounter;

    uint32_t mScore;
    uint32_t mLevel;
    uint32_t mLines;
    uint32_t mCurrentLevelLines;

    // menu-related variables
    Menu mMainMenuScreen;
    Menu mSetupMenuScreen;
    Menu* mCurrentMenuScreen;

    ParameterMenuOptionPtr mLevelMenuOption;

    void SwitchMenuCallback(Menu* newMenu);
    void ExitGameMenuCallback();
    void StartGameCallback();

    void OnEvent(INPUT_RECORD* event);

    void AdvanceBlock();
    void AddBlockToField();
    void UpdateGame(double delta);
    void ProcessGameInput(uint32_t keyCode);
    void AddScore(const uint32_t clearedRows);
    void DrawGame();

    void ProcessMenuInput(uint32_t keyCode);
    void DrawMenu(uint32_t menuOffsetX, uint32_t menuOffsetY);

public:
    Game();
    ~Game();

    bool Create();
    bool SwitchToGameMode(uint32_t fieldX, uint32_t fieldY);
    bool SwitchToMenuMode();
    void MainLoop();
};
