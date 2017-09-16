#include "PCH.hpp"
#include "Game.hpp"
#include "BlockLibrary.hpp"
#include "CallbackMenuOption.hpp"


namespace {

const double BLOCK_FALL_BASE_INTERVAL = 1.0;
const double BLOCK_FALL_PER_LEVEL_INTERVAL_DECREASE = 0.1;
const double ANIMATION_INTERVAL = 0.1;
const double GAME_REFRESH_RATE_S = 0.01;
const uint32_t SCORE_BASE[4] = { 40, 100, 300, 1200};
const uint32_t ROWS_PER_LEVEL = 20;

const char* LOGO[] = {
" ===   ===  =   =  ====  ===  =    ====",
"=   = =   = ==  = =     =   = =    =    ",
"=     =   = = = =  ===  =   = =    ====",
"=   = =   = =  ==     = =   = =    =    ",
" ===   ===  =   = ====   ===  ==== ====",
" ",
"       ===  =   = ====  ====  ====",
"      =   = =   = =   = =    =    ",
"      =     =   = ====  ====  === ",
"      =   = =   = =   = =        =",
"       ===   ===  ====  ==== ==== ",
};
const uint32_t LOGO_ROWS = sizeof(LOGO) / sizeof(LOGO[0]);

} // namespace

Game::Game()
    : mFieldOffsetX(2)
    , mFieldOffsetY(2)
    , mNeedsRedraw(true)
    , mCleanRowAnim(false)
    , mCurrentBlock(Blocks::Collection[0])
    , mNextBlock(Blocks::Collection[1])
    , mNextBlockInd(2)
    , mScore(0)
    , mLevel(0)
    , mLines(0)
    , mCurrentLevelLines(0)
    , mCurrentMenuScreen(nullptr)
{
}

Game::~Game()
{
}


//// menu callbacks ////

void Game::SwitchMenuCallback(Menu* newMenu)
{
    mCurrentMenuScreen = newMenu;
    mCurrentMenuScreen->SelectOption(0);
    mConsole.Clear();
}

void Game::ExitGameMenuCallback()
{
    mMainLoopActive = false;
}

void Game::StartGameCallback()
{
    mConsole.Clear();
    SwitchToGameMode(10, 20);
}


//// window processing ////

void Game::OnEvent(INPUT_RECORD* ev)
{
    switch (ev->EventType)
    {
    case KEY_EVENT:
    {
        WORD vk = ev->Event.KeyEvent.wVirtualKeyCode;
        if (ev->Event.KeyEvent.bKeyDown && !mCleanRowAnim)
        {
            if (mCurrentMode == GameMode::GAME)
                ProcessGameInput(vk);
            else
                ProcessMenuInput(vk);
        }

        break;
    }
    default:
        break;
    }
}


//// game logic ////

void Game::AdvanceBlock()
{
    mCurrentBlock = mNextBlock;
    mCurrentBlock->SetPos(5, 1);

    mNextBlock = Blocks::Collection[mNextBlockInd];
    mNextBlockInd++;
    if (mNextBlockInd >= Blocks::CollectionSize)
        mNextBlockInd = 0;
    mNextBlock->Reset();
    mNextBlock->SetPos(1,1);
}

void Game::AddBlockToField()
{
    mField.PutBlock(mCurrentBlock);

    mField.GetRowsToClean(mClearedRows, &mClearedRowsCount);
    if (mClearedRowsCount)
    {
        mCurrentBlock = nullptr;

        // trigger the animation and initialize params
        mCleanRowAnim = true;
        mAnimationStep = 0;
        mAnimationCounter = 0.0;
        mBlockFallTime = 0.0;
        return;
    }

    AdvanceBlock();
    mNeedsRedraw = true;
}

void Game::UpdateGame(double delta)
{
    if (mCleanRowAnim)
    {
        mAnimationCounter += delta;
        if (mAnimationCounter > ANIMATION_INTERVAL)
        {
            if (mAnimationStep < mField.GetSizeX())
                for (auto& row : mClearedRows)
                    mField.SetFieldCell(mAnimationStep + 1, row, EMPTY_FIELD);

            mNeedsRedraw = true;
            if (mAnimationStep > mField.GetSizeX())
            {
                mCleanRowAnim = false;
                mField.ShiftRowsDown(mClearedRows, mClearedRowsCount);
                AdvanceBlock();
                AddScore(mClearedRowsCount);
                return;
            }

            mAnimationStep++;
            mAnimationCounter -= ANIMATION_INTERVAL;
        }

        return;
    }

    mBlockFallTime += delta;
    double blockFallInterval = BLOCK_FALL_BASE_INTERVAL - BLOCK_FALL_PER_LEVEL_INTERVAL_DECREASE * mLevel;
    if (mBlockFallTime > blockFallInterval)
    {
        uint32_t bPosX = 0, bPosY = 0;
        mCurrentBlock->GetPos(bPosX, bPosY);

        if (!mField.CanGoDown(mCurrentBlock))
        {
            AddBlockToField();
            return;
        }
        else
        {
            bPosY++;
            mCurrentBlock->SetPos(bPosX, bPosY);
            mNeedsRedraw = true;
        }

        mBlockFallTime -= blockFallInterval;
    }
}

void Game::AddScore(const uint32_t clearedRows)
{
    if (clearedRows > 4 || clearedRows == 0)
        return;

    // Scoring system is based on original Nintendo scoring system
    // http://tetris.wikia.com/wiki/Scoring#Original_Nintendo_Scoring_System
    mScore += mLevel * SCORE_BASE[clearedRows - 1];
    mLines += clearedRows;
    mCurrentLevelLines += clearedRows;

    if (mCurrentLevelLines >= ROWS_PER_LEVEL && mLevel < 9)
    {
        mCurrentLevelLines -= ROWS_PER_LEVEL;
        mLevel++;
    }
}

void Game::ProcessGameInput(uint32_t keyCode)
{
    uint32_t bSizeX = 0, bSizeY = 0;
    uint32_t bPosX = 0, bPosY = 0;

    mCurrentBlock->GetPos(bPosX, bPosY);
    mCurrentBlock->GetSize(bSizeX, bSizeY);

    if (keyCode == VK_LEFT && mField.CanGoLeft(mCurrentBlock))
        bPosX--;
    if (keyCode == VK_RIGHT && mField.CanGoRight(mCurrentBlock))
        bPosX++;
    if (keyCode == VK_DOWN)
    {
        if (mField.CanGoDown(mCurrentBlock))
        {
            bPosY++;
            mBlockFallTime = 0.0;
        }
        else
        {
            AddBlockToField();
            return;
        }
    }
    if (keyCode == VK_UP)
    {
        if (mField.CanAdvanceVariant(mCurrentBlock))
        {
            mCurrentBlock->NextVariant();
            mCurrentBlock->GetSize(bSizeX, bSizeY);

            if (bPosX + bSizeX > mField.GetSizeX())
                bPosX = mField.GetSizeX() - bSizeX + 1;
        }
    }

    mCurrentBlock->SetPos(bPosX, bPosY);
    mNeedsRedraw = true; 
}

void Game::DrawGame()
{
    mField.Draw(mFieldOffsetX, mFieldOffsetY, mCurrentBlock);
    mNextBlockField.Draw(mFieldOffsetX + mField.GetSizeX() + 3, mFieldOffsetY, mNextBlock);

    mConsole.SetPosition(mFieldOffsetX + mField.GetSizeX() + 3,
                            mFieldOffsetY + mNextBlockField.GetSizeY() + 3);
    mConsole.Write("Score: " + std::to_string(mScore));

    mConsole.SetPosition(mFieldOffsetX + mField.GetSizeX() + 3,
                            mFieldOffsetY + mNextBlockField.GetSizeY() + 5);
    mConsole.Write("Level: " + std::to_string(mLevel));

    mConsole.SetPosition(mFieldOffsetX + mField.GetSizeX() + 3,
                            mFieldOffsetY + mNextBlockField.GetSizeY() + 7);
    mConsole.Write("Lines: " + std::to_string(mLines));

    mNeedsRedraw = false;
}


//// menu processing ////

void Game::ProcessMenuInput(uint32_t keyCode)
{
    mCurrentMenuScreen->Update(keyCode);

    int32_t option = mCurrentMenuScreen->GetSelectedOption();

    if (keyCode == VK_DOWN)
        option++;
    if (keyCode == VK_UP)
        option--;

    mCurrentMenuScreen->SelectOption(option);

    mNeedsRedraw = true;
}

void Game::DrawMenu(uint32_t menuOffsetX, uint32_t menuOffsetY)
{
    // logo
    for (uint32_t j = 0; j < LOGO_ROWS; ++j)
    {
        mConsole.SetPosition(menuOffsetX, menuOffsetY + j);
        mConsole.Write(LOGO[j]);
    }

    // actual menu
    if (mCurrentMenuScreen == nullptr)
    {
        mConsole.SetPosition(0, 0);
        mConsole.Write("Incorrect Menu Screen to be drawn - this should not happen");
    }
    mCurrentMenuScreen->Draw(menuOffsetX, menuOffsetY + LOGO_ROWS + 3);

    mNeedsRedraw = false;
}


//// globals ////

bool Game::Create()
{
    if (!mConsole.Create())
        return false;

    mConsole.SetEventCallback(std::bind(&Game::OnEvent, this, std::placeholders::_1));

    MenuOptions mainMenuOptions;
    mainMenuOptions.push_back(std::make_shared<CallbackMenuOption>("Start Game", std::bind(&Game::SwitchMenuCallback, this, &mSetupMenuScreen)));
    mainMenuOptions.push_back(std::make_shared<CallbackMenuOption>("Exit", std::bind(&Game::ExitGameMenuCallback, this)));
    mMainMenuScreen.Create(&mConsole, mainMenuOptions);

    mLevelMenuOption.reset(new ParameterMenuOption("Level", 0, 9));

    MenuOptions setupMenuOptions;
    setupMenuOptions.push_back(std::make_shared<CallbackMenuOption>("Start", std::bind(&Game::StartGameCallback, this)));
    setupMenuOptions.push_back(mLevelMenuOption);
    setupMenuOptions.push_back(std::make_shared<CallbackMenuOption>("Exit", std::bind(&Game::SwitchMenuCallback, this, &mMainMenuScreen)));
    mSetupMenuScreen.Create(&mConsole, setupMenuOptions);

    mMainLoopActive = true;

    return true;
}

bool Game::SwitchToGameMode(uint32_t fieldX, uint32_t fieldY)
{
    if (mCurrentMode == GameMode::GAME)
        return true;

    mCurrentMode = GameMode::GAME;

    if (!mField.Create(&mConsole, fieldX, fieldY))
        return false;

    mCurrentBlock = Blocks::Collection[0];
    mNextBlock = Blocks::Collection[1];
    mNextBlockInd = 2;

    if (!mNextBlockField.Create(&mConsole, 4, 4))
        return false;

    mNextBlock->SetPos(2, 2);

    mBlockFallTime = 0.0;

    mScore = 0;
    mLevel = static_cast<uint32_t>(mLevelMenuOption->GetValue());
    mLines = 0;
    mCurrentLevelLines = 0;

    return true;
}

bool Game::SwitchToMenuMode()
{
    if (mCurrentMode == GameMode::MENU)
        return true;

    mCurrentMode = GameMode::MENU;

    mField.Destroy();
    mNextBlockField.Destroy();

    mCurrentBlock = nullptr;
    mNextBlock = nullptr;
    mNextBlockInd = 0;

    mCurrentMenuScreen = &mMainMenuScreen;
    return true;
}

void Game::MainLoop()
{
    mGameTimer.Start();

    while (mMainLoopActive)
    {
        double delta = mGameTimer.Stop();
        mGameTimer.Start();

        // simulate 100fps vsync, without this the CPU fries for no reason at all
        double sleepDuration = GAME_REFRESH_RATE_S - delta;
        if (sleepDuration > 0.0)
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepDuration));

        mConsole.EventLoop();

        if (mCurrentMode == GameMode::GAME)
        {
            UpdateGame(delta);

            if (mNeedsRedraw)
                DrawGame();
        }
        else if (mCurrentMode == GameMode::MENU)
        {
            if (mNeedsRedraw)
                DrawMenu(5, 2);
        }
    }
}
