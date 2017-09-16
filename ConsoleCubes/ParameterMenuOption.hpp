#pragma once

#include "MenuOption.hpp"


class ParameterMenuOption: public MenuOption
{
    int32_t mMinValue;
    int32_t mMaxValue;
    int32_t mCurrentValue;

public:
    ParameterMenuOption(const std::string& optionStr, int32_t minValue, int32_t maxValue);
    void Update(uint32_t keyCode);
    void Draw(Console* console, uint32_t x, uint32_t y);

    __forceinline int32_t GetValue() const
    {
        return mCurrentValue;
    }
};

using ParameterMenuOptionPtr = std::shared_ptr<ParameterMenuOption>;
