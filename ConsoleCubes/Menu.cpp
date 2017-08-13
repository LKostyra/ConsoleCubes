#include "PCH.hpp"
#include "Menu.hpp"

Menu::Menu()
    : mSelectedOption(0)
    , mOptions()
{
}

Menu::~Menu()
{
}

void Menu::Create(Console* console, const std::vector<MenuOption>& options)
{
    mConsole = console;
    mOptions = options;
    mSelectedOption = 0;
}

void Menu::Release()
{
    mConsole = nullptr;
    mOptions.clear();
    mSelectedOption = 0;
}

void Menu::Draw(uint32_t offsetX, uint32_t offsetY)
{
    uint32_t height = offsetY;
    for (uint32_t i = 0; i < mOptions.size(); ++i)
    {
        if (i == mSelectedOption)
        {
            mConsole->SetPosition(offsetX, height);
            mConsole->Write("> " + mOptions[i].name);
        }
        else
        {
            mConsole->SetPosition(offsetX, height);
            mConsole->Write("  " + mOptions[i].name);
        }

        if (mOptions[i].maxValue >= 0)
            mConsole->Write(": " + std::to_string(mOptions[i].value));

        height += 2;
    }
}
