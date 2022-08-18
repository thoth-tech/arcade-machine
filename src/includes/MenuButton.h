#ifndef ARCADE_MACHINE_MENU_BUTTON_H
#define ARCADE_MACHINE_MENU_BUTTON_H

#include <string>
#include "Button.h"

/**
* @brief Buttons created for the main opening Menu Screen
* 
* Derived from abstract Button class
*/
class MenuButton : public Button
{
public:
    MenuButton(Color c, float scale = 1);

    void getButtonImage(std::string image) {} //?

    void btnImage(std::string image) override {}
    void drawButton() override; 
    std::string action(std::string keyinput = "") override;

};

#endif