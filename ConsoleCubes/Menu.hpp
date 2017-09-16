#pragma once

#include "Console.hpp"
#include "MenuOption.hpp"

using MenuOptions = std::vector<std::shared_ptr<MenuOption>>;

class Menu final
{
protected:
    Console* mConsole;
    int32_t mSelectedOption;
    MenuOptions mOptions;

public:
    Menu();
    ~Menu();

    void Create(Console* console, const MenuOptions& options);
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

    __forceinline void Update(uint32_t keyCode)
    {
        mOptions[mSelectedOption]->Update(keyCode);
    }
};
