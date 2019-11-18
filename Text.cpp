#include "Text.hpp"

Text::Text(sf::Font& fnt, const char* text, int size,
           sf::Color colour, const sf::Vector2f& pos)
    : x(pos.x), y(pos.y)
{
    setFont(fnt);
    setText(text);
    setSize(size);
    setColour(colour);
}

void Text::draw(sf::RenderWindow& window)
{
    // Center text
    sf::FloatRect labelRect = label.getLocalBounds();
    label.setPosition(x - labelRect.width / 2, y);
    //labelRect.
    window.draw(label);
}

