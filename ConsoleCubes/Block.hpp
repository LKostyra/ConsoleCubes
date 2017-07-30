#pragma once


struct BlockDesc
{
    unsigned char* shape;
    uint32_t sizeX;
    uint32_t sizeY;
};

class Block
{
    std::vector<BlockDesc> mVariants;
    uint32_t mCurrentVariant;
    uint32_t mPosX;
    uint32_t mPosY;

public:
    Block(std::vector<BlockDesc> variants);
    ~Block();

    void Reset();

    __forceinline void SetPos(uint32_t x, uint32_t y)
    {
        mPosX = x;
        mPosY = y;
    }

    __forceinline void GetPos(uint32_t& x, uint32_t& y) const
    {
        x = mPosX;
        y = mPosY;
    }

    __forceinline void GetSize(uint32_t& x, uint32_t& y, bool fromNext = false) const
    {
        uint32_t variant = fromNext ? mCurrentVariant + 1 : mCurrentVariant;
        if (variant >= mVariants.size())
            variant = 0;

        x = mVariants[variant].sizeX;
        y = mVariants[variant].sizeY;
    }

    __forceinline unsigned char GetShapeCell(uint32_t x, uint32_t y, bool fromNext = false) const
    {
        uint32_t variant = fromNext ? mCurrentVariant + 1 : mCurrentVariant;
        if (variant >= mVariants.size())
            variant = 0;

        return mVariants[variant].shape[y * mVariants[variant].sizeX + x];
    }

    __forceinline void NextVariant()
    {
        if (++mCurrentVariant == mVariants.size())
            mCurrentVariant = 0;
    }
};
