#include "PCH.hpp"
#include "Block.hpp"

Block::Block(std::vector<BlockDesc> variants)
    : mPosX(1)
    , mPosY(1)
    , mCurrentVariant(0)
{
    mVariants = variants;
}

Block::~Block()
{
}

void Block::Reset()
{
    mCurrentVariant = 0;
}
