#include "PCH.hpp"
#include "Game.hpp"
#include "BlockLibrary.hpp"


void Game::OnEvent(INPUT_RECORD* ev)
{
    switch (ev->EventType)
    {
    case KEY_EVENT:
    {
        WORD vk = ev->Event.KeyEvent.wVirtualKeyCode;
        if (ev->Event.KeyEvent.bKeyDown)
        {
            UpdateGame(vk);
            mNeedsRedraw = true;
        }

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
    , mCurrentBlock(Blocks::Collection[0])
    , mNextBlock(Blocks::Collection[1])
    , mNextBlockInd(2)
{
}

Game::~Game()
{
}

void Game::UpdateGame(uint32_t keyCode)
{
    if (!mField.CanGoDown(mCurrentBlock))
    {
        mField.PutBlock(mCurrentBlock);
        mCurrentBlock = mNextBlock;
        mCurrentBlock->SetPos(5, 1);
        mNextBlock = Blocks::Collection[mNextBlockInd];
        mNextBlockInd++;
        if (mNextBlockInd >= Blocks::CollectionSize)
            mNextBlockInd = 0;
        mNextBlock->SetPos(1,1);
        return;
    }

    uint32_t bSizeX = 0, bSizeY = 0;
    uint32_t bPosX = 0, bPosY = 0;

    mCurrentBlock->GetPos(bPosX, bPosY);
    mCurrentBlock->GetSize(bSizeX, bSizeY);

    // position update
    if (keyCode == VK_LEFT && mField.CanGoLeft(mCurrentBlock))
        bPosX--;
    if (keyCode == VK_RIGHT && mField.CanGoRight(mCurrentBlock))
        bPosX++;
    if (keyCode == VK_DOWN)
        bPosY++;
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

    return true;
}

void Game::MainLoop()
{
    while (mConsole.EventLoop())
    {
        if (mCurrentMode == GameMode::GAME)
        {
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
