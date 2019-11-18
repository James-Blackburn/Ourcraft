#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "Text.hpp"

#include <vector>

int main()
{
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    Game ourcraft(modes[0].width, modes[0].height);
    while (ourcraft.window.isOpen())
    {
        ourcraft.gameLoop();
    }

    return 0;
}
