#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Text.hpp"

#include <string>

int main()
{
    Game ourcraft;
    while (ourcraft.window.isOpen())
    {
        ourcraft.gameLoop();
    }

    return 0;
}
