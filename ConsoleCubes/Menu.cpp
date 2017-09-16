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

void Menu::Create(Console* console, const MenuOptions& options)
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
            mConsole->Write("> ");
        }
        else
        {
            mConsole->SetPosition(offsetX, height);
            mConsole->Write("  "); // will hide previous "> " selector
        }

        mOptions[i]->Draw(mConsole, offsetX + 2, height);
        height += 2;
    }
}
