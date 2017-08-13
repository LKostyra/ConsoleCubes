#include "PCH.hpp"
#include "Game.hpp"


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Game g;
    g.Create();
    g.SwitchToMenuMode();
    g.MainLoop();
    return 0;
}
