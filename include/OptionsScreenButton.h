#ifndef ARCADE_MACHINE_OPTIONS_SCREEN_BUTTON_H
#define ARCADE_MACHINE_OPTIONS_SCREEN_BUTTON_H

#include <string>
#include "Button.h"

/**
* @brief Buttons created for Options Screen Menu
* 
* Derived from abstract Button class
*/
class OptionsScreenButton : public Button
{
public:
    OptionsScreenButton(Color c, float scale = 1);

    void btnImage(std::string image) override;
    void drawButton() override; 
    std::string action(std::string keyinput = "") override;

};

#endif