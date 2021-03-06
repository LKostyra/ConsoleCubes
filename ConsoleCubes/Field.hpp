#pragma once

#include "Console.hpp"
#include "Block.hpp"

extern const unsigned char EMPTY_FIELD;
extern const unsigned char TAKEN_FIELD;
extern const unsigned char WALL_FIELD;

class Field
{
    Console* mConsole;
    uint32_t mFieldSizeX;
    uint32_t mFieldSizeY;
    std::vector<unsigned char> mField;

    __forceinline unsigned char GetFieldCell(uint32_t x, uint32_t y) const
    {
        return mField[y * mFieldSizeX + x];
    }

public:
    Field();
    ~Field();

    bool Create(Console* con, uint32_t fieldX, uint32_t fieldY);
    void Destroy();
    void Draw(uint32_t x, uint32_t y, Block* b);

    void PutBlock(Block* b);
    bool CanGoDown(Block* b);
    bool CanGoRight(Block* b);
    bool CanGoLeft(Block* b);
    bool CanAdvanceVariant(Block* b);

    void GetRowsToClean(uint32_t* rows, uint32_t* rowCount) const;
    void ShiftRowsDown(uint32_t* rows, uint32_t rowCount);

    __forceinline uint32_t GetSizeX() const
    {
        return mFieldSizeX - 2;
    }

    __forceinline uint32_t GetSizeY() const
    {
        return mFieldSizeY - 2;
    }

    __forceinline void SetFieldCell(uint32_t x, uint32_t y, unsigned char c)
    {
        mField[y * mFieldSizeX + x] = c;
    }
};