#include "PCH.hpp"
#include "Field.hpp"

const unsigned char EMPTY_FIELD = 0xB0;
const unsigned char TAKEN_FIELD = 0x01;
const unsigned char WALL_FIELD = 0xDB;

Field::Field()
{
}

Field::~Field()
{
}

bool Field::Create(Console* con, uint32_t fieldX, uint32_t fieldY)
{
    mConsole = con;

    mFieldSizeX = fieldX + 2;
    mFieldSizeY = fieldY + 2;

    mField.resize(mFieldSizeX * mFieldSizeY);

    // build base field
    for (uint32_t j = 0; j < mFieldSizeY; ++j)
    {
        for (uint32_t i = 0; i < mFieldSizeX; ++i)
        {
            if (i == 0 || j == 0 ||
                i == mFieldSizeX - 1 || j == mFieldSizeY - 1)
                SetFieldCell(i, j, WALL_FIELD);
            else
                SetFieldCell(i, j, EMPTY_FIELD);
        }
    }

    return true;
}

void Field::Destroy()
{
    mFieldSizeX = 0;
    mFieldSizeY = 0;
    mField.clear();
    mConsole = nullptr;
}

void Field::Draw(uint32_t x, uint32_t y, Block* b)
{
    for (uint32_t j = 0; j < mFieldSizeY; ++j)
    {
        mConsole->SetPosition(x, y + j);
        for (uint32_t i = 0; i < mFieldSizeX; ++i)
        {
            mConsole->Write(mField[(j * mFieldSizeX) + i]);
        }
    }

    if (b)
    {
        uint32_t bPosX = 0, bPosY = 0;
        uint32_t bSizeX = 0, bSizeY = 0;

        b->GetPos(bPosX, bPosY);
        b->GetSize(bSizeX, bSizeY);

        for (uint32_t j = 0; j < bSizeY; ++j)
        {
            for (uint32_t i = 0; i < bSizeX; ++i)
            {
                mConsole->SetPosition(x + bPosX + i, y + bPosY + j);
                unsigned char cell = b->GetShapeCell(i, j);
                if (cell != 0)
                    mConsole->Write(TAKEN_FIELD);
            }
        }
    }
}

void Field::PutBlock(Block* b)
{
    uint32_t bPosX = 0, bPosY = 0;
    uint32_t bSizeX = 0, bSizeY = 0;

    b->GetPos(bPosX, bPosY);
    b->GetSize(bSizeX, bSizeY);

    for (uint32_t y = 0; y < bSizeY; ++y)
        for (uint32_t x = 0; x < bSizeX; ++x)
            if (b->GetShapeCell(x, y) != 0)
                SetFieldCell(bPosX + x, bPosY + y, TAKEN_FIELD);
}

// Check if block can go down field
// This basically asks whether there is a column of block's shape
// which on field has taken blocks below it.
// This function should be called each time a block goes down.
bool Field::CanGoDown(Block* b)
{
    uint32_t bPosX = 0, bPosY = 0;
    uint32_t bSizeX = 0, bSizeY = 0;

    b->GetPos(bPosX, bPosY);
    b->GetSize(bSizeX, bSizeY);

    // check each column of the box
    for (uint32_t i = 0; i < bSizeX; ++i)
    {
        uint32_t lowestPos = bPosY;
        // find lowest point of block's shape
        for (int32_t j = static_cast<int32_t>(bSizeY - 1); j >= 0; --j)
            if (b->GetShapeCell(i, j) != 0)
            {
                lowestPos = bPosY + j;
                break;
            }

        // check if position below lowestPos is empty
        if (GetFieldCell(bPosX + i, lowestPos + 1) != EMPTY_FIELD)
            return false;
    }

    return true;
}

// Check if block can be moved right.
// Works similarly to PutBlock, however in X axis.
// Should be called each time player presses right arrow key.
bool Field::CanGoRight(Block* b)
{
    uint32_t bPosX = 0, bPosY = 0;
    uint32_t bSizeX = 0, bSizeY = 0;

    b->GetPos(bPosX, bPosY);
    b->GetSize(bSizeX, bSizeY);

    for (uint32_t j = 0; j < bSizeY; ++j)
    {
        uint32_t furthestPos = bPosX;
        for (int32_t i = static_cast<int32_t>(bSizeX - 1); i >= 0; --i)
        {
            if (b->GetShapeCell(i, j) != 0)
            {
                furthestPos = bPosX + i;
                break;
            }
        }

        if (GetFieldCell(furthestPos + 1, bPosY + j) != EMPTY_FIELD)
            return false;
    }

    return true;
}

// Check if block can be moved right.
// As above, but with different direction
bool Field::CanGoLeft(Block* b)
{
    uint32_t bPosX = 0, bPosY = 0;
    uint32_t bSizeX = 0, bSizeY = 0;

    b->GetPos(bPosX, bPosY);
    b->GetSize(bSizeX, bSizeY);

    for (uint32_t j = 0; j < bSizeY; ++j)
    {
        uint32_t furthestPos = bPosX + bSizeX;
        for (uint32_t i = 0; i < bSizeX; ++i)
        {
            if (b->GetShapeCell(i, j) != 0)
            {
                furthestPos = bPosX + i;
                break;
            }
        }

        if (GetFieldCell(furthestPos - 1, bPosY + j) != EMPTY_FIELD)
            return false;
    }

    return true;
}

bool Field::CanAdvanceVariant(Block* b)
{
    uint32_t bPosX = 0, bPosY = 0;
    uint32_t bSizeX = 0, bSizeY = 0;

    b->GetPos(bPosX, bPosY);
    b->GetSize(bSizeX, bSizeY, true);

    // adjust position to simulate moving away from wall
    if (bPosX + bSizeX >= GetSizeX())
        bPosX = GetSizeX() - bSizeX + 1;

    // go through our block and check if non-empty pieces overlap with field
    for (uint32_t j = 0; j < bSizeY; ++j)
    {
        for (uint32_t i = 0; i < bSizeX; ++i)
        {
            if (b->GetShapeCell(i, j, true) != 0 && GetFieldCell(bPosX + i, bPosY + j) != EMPTY_FIELD)
                    return false;
        }
    }

    return true;
}

// gathers which rows are full and need to be removed from the playfield
// rowCount shouldn't exceed 4
void Field::GetRowsToClean(uint32_t* rows, uint32_t* rowCount) const
{
    if (!rows || !rowCount)
        return;

    for (uint32_t j = 1; j <= GetSizeY(); ++j)
    {
        bool hasEmptySpaces = false;

        for (uint32_t i = 1; i <= GetSizeX(); ++i)
        {
            if (GetFieldCell(i, j) == EMPTY_FIELD)
            {
                hasEmptySpaces = true;
                break;
            }
        }

        if (!hasEmptySpaces)
        {
            rows[*rowCount] = j;
            *rowCount++;
        }
    }
}

void Field::ShiftRowsDown(uint32_t* rows, uint32_t rowCount)
{
    for (uint32_t r = 0; r < rowCount; ++r)
    {
        for (uint32_t j = rows[r]; j > 0; --j)
        {
            for (uint32_t i = 1; i <= GetSizeX(); ++i)
            {
                SetFieldCell(i, j, GetFieldCell(i, j - 1));
            }
        }

        for (uint32_t i = 1; i <= GetSizeX(); ++i)
        {
            SetFieldCell(i, 1, EMPTY_FIELD);
        }
    }
}
