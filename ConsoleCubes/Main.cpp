#include "PCH.hpp"
#include "Game.hpp"


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Game g;
    g.Create();
    g.SwitchToGameMode(10, 20);
    g.MainLoop();
    return 0;
}