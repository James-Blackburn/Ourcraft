#include "Text.hpp"

Text::Text(sf::Font& fnt, const char* text, int size,
           sf::Color colour, const sf::Vector2f& pos)
{
    setFont(fnt);
    setText(text);
    setSize(size);
    setColour(colour);
    setPos(pos);
}

void Text::draw(sf::RenderWindow& window)
{
    window.draw(label);
}

