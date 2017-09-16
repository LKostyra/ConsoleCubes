#include "PCH.hpp"
#include "ParameterMenuOption.hpp"


ParameterMenuOption::ParameterMenuOption(const std::string& optionStr, int32_t minValue, int32_t maxValue)
    : MenuOption(optionStr)
    , mMinValue(minValue)
    , mMaxValue(maxValue)
    , mCurrentValue(minValue)
{
}

void ParameterMenuOption::Update(uint32_t keyCode)
{
    if (keyCode == VK_LEFT && mCurrentValue != mMinValue)
        mCurrentValue--;
    if (keyCode == VK_RIGHT && mCurrentValue != mMaxValue)
        mCurrentValue++;
}

void ParameterMenuOption::Draw(Console* console, uint32_t x, uint32_t y)
{
    console->SetPosition(x, y);
    console->Write(mOptionString + ": " + std::to_string(mCurrentValue));
}
