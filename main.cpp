#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Text.hpp"

int main()
{
    Game ourcraft;
    while (ourcraft.window.isOpen())
    {
        ourcraft.gameLoop();
    }

    return 0;
}
