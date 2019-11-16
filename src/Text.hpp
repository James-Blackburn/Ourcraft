#ifndef TEXT_HPP_INCLUDED
#define TEXT_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>

class Text
{
private:
    sf::Text label;
    int x, y;
public:
    Text(sf::Font& fnt, const char* text, int size,
          sf::Color colour, const sf::Vector2f& pos);
    ~Text() = default;

    inline void setFont(sf::Font& fnt){label.setFont(fnt); }
    inline void setText(const char* text){ label.setString(text); }
    inline void setText(const std::string& text){ label.setString(text); }
    inline void setSize(int size){ label.setCharacterSize(size); }
    inline void setColour(sf::Color colour){ label.setColor(colour); }
    inline void setPos(const sf::Vector2f& pos){ label.setPosition(pos.x,pos.y); }

    void draw(sf::RenderWindow& window);
};

#endif // TEXT_HPP_INCLUDED
