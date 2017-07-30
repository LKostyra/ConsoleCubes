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

    Block* mCurrentBlock;
    Block* mNextBlock;
    uint32_t mNextBlockInd;

    void OnEvent(INPUT_RECORD* event);

    void UpdateGame(uint32_t keyCode);
    void UpdateMenu();

public:
    Game();
    ~Game();

    bool Create();
    bool SwitchToGameMode(uint32_t fieldX, uint32_t fieldY);
    void MainLoop();
};
