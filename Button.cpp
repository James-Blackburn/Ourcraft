#include "Button.hpp"

Button::Button(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& colour)
{
    setPosition(pos);
    setSize(size);
    setColour(colour);
}

bool Button::clicked(const sf::Vector2f& mpos)
{
    sf::FloatRect boundingRect = rectangle.getGlobalBounds();
    if (boundingRect.contains(mpos)) return true;
    return false;
}

void Button::draw(sf::RenderWindow& window)
{
    window.draw(rectangle);
}
