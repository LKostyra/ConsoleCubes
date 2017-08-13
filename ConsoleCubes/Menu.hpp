#pragma once

#include "Console.hpp"


struct MenuOption
{
    std::string name;
    std::function<void()> callback;
    int32_t value;
    int32_t maxValue;

    MenuOption()
        : name()
        , callback()
        , value(-1)
        , maxValue(-1)
    {
    }

    MenuOption(const std::string& name, std::function<void()> callback, int32_t value = -1, int32_t maxValue = -1)
        : name(name)
        , callback(callback)
        , value(value)
        , maxValue(maxValue)
    {
    }
};

class Menu final
{
protected:
    Console* mConsole;
    int32_t mSelectedOption;
    std::vector<MenuOption> mOptions;

public:
    Menu();
    ~Menu();

    void Create(Console* console, const std::vector<MenuOption>& options);
    void Draw(uint32_t offsetX, uint32_t offsetY);
    void Release();

    __forceinline int32_t GetSelectedOption() const
    {
        return mSelectedOption;
    }

    __forceinline void SelectOption(int32_t option)
    {
        if (option < 0)
            mSelectedOption = 0;
        else if (option >= static_cast<int32_t>(mOptions.size()))
            mSelectedOption = static_cast<int32_t>(mOptions.size()) - 1;
        else
            mSelectedOption = option;
    }

    __forceinline uint32_t GetOptionValue() const
    {
        return mOptions[mSelectedOption].value;
    }

    __forceinline void SetOptionValue(uint32_t value)
    {
        mOptions[mSelectedOption].value = value;
    }

    __forceinline void ExecuteSelectedOption()
    {
        mOptions[mSelectedOption].callback();
    }
};
