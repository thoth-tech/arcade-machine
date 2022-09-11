#ifndef ARCADE_MACHINE_GAME_SCREEN_BUTTON_H
#define ARCADE_MACHINE_GAME_SCREEN_BUTTON_H

#include "Button.h"
#include <string>

/**
 * @brief Buttons created for Game Screen Menu
 *
 * Derived from abstract Button class
 */
class GameScreenButton : public Button
{
public:
    GameScreenButton(Color c, float scale = 1);
    GameScreenButton(Color c, std::string s, float scale = 1);

    void btnImage(std::string image) override;
    void drawButton() override;
    std::string action(std::string keyinput = "") override;
};

#endif