#include "PCH.hpp"
#include "Game.hpp"
#include "BlockLibrary.hpp"


namespace {

const double BLOCK_FALL_INTERVAL = 1.0;
const double ANIMATION_INTERVAL = 0.1;

} // namespace

void Game::OnEvent(INPUT_RECORD* ev)
{
    switch (ev->EventType)
    {
    case KEY_EVENT:
    {
        WORD vk = ev->Event.KeyEvent.wVirtualKeyCode;
        if (ev->Event.KeyEvent.bKeyDown && !mCleanRowAnim)
            OnPlayerInput(vk);

        break;
    }
    default:
        break;
    }
}


Game::Game()
    : mFieldOffsetX(2)
    , mFieldOffsetY(2)
    , mNeedsRedraw(true)
    , mCleanRowAnim(false)
    , mCurrentBlock(Blocks::Collection[0])
    , mNextBlock(Blocks::Collection[1])
    , mNextBlockInd(2)
{
}

Game::~Game()
{
}

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

    mField.GetRowsToClean(mAnimationRows);
    if (!mAnimationRows.empty())
    {
        mCurrentBlock = nullptr;
        mCleanRowAnim = true;
        mAnimationStep = 0;
        mAnimationCounter = 0.0;
        mBlockFallTime = 0.0;
        return;
    }

    AdvanceBlock();
    mNeedsRedraw = true;
}

void Game::UpdateGame()
{
    double delta = mGameTimer.Stop();
    mGameTimer.Start();

    if (mCleanRowAnim)
    {
        mAnimationCounter += delta;
        if (mAnimationCounter > ANIMATION_INTERVAL)
        {
            if (mAnimationStep < mField.GetSizeX())
                for (auto& row : mAnimationRows)
                    mField.SetFieldCell(mAnimationStep + 1, row, EMPTY_FIELD);

            mNeedsRedraw = true;
            if (mAnimationStep > mField.GetSizeX())
            {
                mCleanRowAnim = false;
                mField.ShiftRowsDown(mAnimationRows);
                AdvanceBlock();
                return;
            }

            mAnimationStep++;
            mAnimationCounter -= ANIMATION_INTERVAL;
        }

        return;
    }

    mBlockFallTime += delta;
    if (mBlockFallTime > BLOCK_FALL_INTERVAL)
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

        mBlockFallTime -= BLOCK_FALL_INTERVAL;
    }
}

void Game::OnPlayerInput(uint32_t keyCode)
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

void Game::UpdateMenu()
{
    // TODO
}

bool Game::Create()
{
    if (!mConsole.Create())
        return false;

    mConsole.SetEventCallback(std::bind(&Game::OnEvent, this, std::placeholders::_1));

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

    mGameTimer.Start();
    mBlockFallTime = 0.0;

    return true;
}

void Game::MainLoop()
{
    while (mConsole.EventLoop())
    {
        if (mCurrentMode == GameMode::GAME)
        {
            UpdateGame();

            // do the drawing
            if (mNeedsRedraw)
            {
                mField.Draw(mFieldOffsetX, mFieldOffsetY, mCurrentBlock);
                mNextBlockField.Draw(mFieldOffsetX + mField.GetSizeX() + 3, mFieldOffsetY, mNextBlock);

                mNeedsRedraw = false;
            }
        }
    }
}
