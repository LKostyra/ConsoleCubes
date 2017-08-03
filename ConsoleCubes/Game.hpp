#pragma once

#include "Console.hpp"
#include "Field.hpp"
#include "Block.hpp"
#include "Timer.hpp"


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

    bool mNeedsRedraw;
    bool mCleanRowAnim;

    Block* mCurrentBlock;
    Block* mNextBlock;
    uint32_t mNextBlockInd;
    double mBlockFallTime;
    
    std::vector<uint32_t> mAnimationRows;
    uint32_t mAnimationStep;
    double mAnimationCounter;

    void OnEvent(INPUT_RECORD* event);

    void AdvanceBlock();
    void AddBlockToField();
    void UpdateGame();
    void OnPlayerInput(uint32_t keyCode);
    void UpdateMenu();

public:
    Game();
    ~Game();

    bool Create();
    bool SwitchToGameMode(uint32_t fieldX, uint32_t fieldY);
    void MainLoop();
};
