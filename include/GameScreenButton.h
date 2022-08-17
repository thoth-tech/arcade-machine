#ifndef ARCADE_MACHINE_GAME_SCREEN_BUTTON_H
#define ARCADE_MACHINE_GAME_SCREEN_BUTTON_H

#include <string>
#include "splashkit.h"
#include "Button.h"

/**
 * @brief Buttons created for Game Screen Menu
 * 
 * Derived from abstract Button class
 */
class GameScreenButton : public Button
{
    public:
        // First constructor
        GameScreenButton(Color c, float scale = 1) : Button(c, scale){}
        // Third constructor
        GameScreenButton(Color c, std::string s, float scale = 1) : Button(c, s, scale){}

        /**
         * @brief returns the action 
         * 
         * @param keyinput 
         * @return string 
         */
        std::string action(std::string keyinput = "")
        {
            return keyinput;
        }

        void btnImage(std::string image)
        {
            this->m_btnColor = image;
        }

        // draw button to screen
        void drawButton()
        {
            draw_sprite(this->m_btn);
        }
};

#endif