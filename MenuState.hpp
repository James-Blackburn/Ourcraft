#ifndef MENUSTATE_HPP_INCLUDED
#define MENUSTATE_HPP_INCLUDED

#include <SFML/Audio.hpp>
#include <memory>

#include "GameState.hpp"

class Button;
class Text;

class MenuState : public GameState
{
private:
    std::unique_ptr<Button> playButton;
    std::unique_ptr<Button> exitButton;
    std::unique_ptr<Text> titleText;
    std::unique_ptr<Text> playText;
    std::unique_ptr<Text> exitText;

public:
    MenuState(Game*);
    ~MenuState();
    void update() override;
    void draw() override;
};

#endif // MENUSTATE_HPP_INCLUDED
