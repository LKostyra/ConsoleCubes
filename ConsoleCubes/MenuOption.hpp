#pragma once

#include "Console.hpp"

class MenuOption
{
protected:
    std::string mOptionString;

public:
    MenuOption(const std::string& optionStr);
    virtual void Update(uint32_t keyCode) = 0;
    virtual void Draw(Console* console, uint32_t x, uint32_t y) = 0;
};
