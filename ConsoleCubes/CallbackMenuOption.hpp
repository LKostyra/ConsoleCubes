#pragma once

#include "MenuOption.hpp"


class CallbackMenuOption: public MenuOption
{
    std::function<void()> mCallback;

public:
    CallbackMenuOption(const std::string& optionStr, std::function<void()> callback);
    void Update(uint32_t keyCode);
    void Draw(Console* console, uint32_t x, uint32_t y);
};
