#ifndef BUTTON_HPP_INCLUDED
#define BUTTON_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Button
{
private:
    sf::RectangleShape rectangle;
public:
    Button(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& colour);

    inline void setSize(const sf::Vector2f& size){ rectangle.setSize(size); }
    inline void setPosition(const sf::Vector2f& pos){ rectangle.setPosition(pos); }
    inline void setColour(const sf::Color& colour){ rectangle.setFillColor(colour); }

    bool clicked(const sf::Vector2f& mpos);
    void draw(sf::RenderWindow& window);
};

#endif // BUTTON_HPP_INCLUDED
