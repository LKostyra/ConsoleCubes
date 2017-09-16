#include "PCH.hpp"
#include "CallbackMenuOption.hpp"
#include "Menu.hpp"


CallbackMenuOption::CallbackMenuOption(const std::string& optionStr, std::function<void()> callback)
    : MenuOption(optionStr)
    , mCallback(callback)
{
}

void CallbackMenuOption::Update(uint32_t keyCode)
{
    if (keyCode == VK_RETURN)
        mCallback();
}

void CallbackMenuOption::Draw(Console* console, uint32_t x, uint32_t y)
{
    console->SetPosition(x, y);
    console->Write(mOptionString);
}
