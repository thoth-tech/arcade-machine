#ifndef ARCADE_MACHINE_MENU_BUTTON_H
#define ARCADE_MACHINE_MENU_BUTTON_H

#include <string>
#include "splashkit.h"
#include "Button.h"

/**
 * @brief Buttons created for the main opening Menu Screen
 * 
 * Derived from abstract Button class
 */
class MenuButton : public Button {
    public:
        // First constructor
        MenuButton(Color c, float scale = 1) : Button(c, scale){}

        /**
         * @brief The action of this button
         * Called when the selector receives input for this button
         * 
         * @param keyinput 
         * @return string
         */
        std::string action(std::string keyinput = "")
        {
            if (this->color() == btn_color(Button::PLAY))
            {
                // go to this screen
                write_line("Play");
                return "play";
            }
            if (this->color() == btn_color(Button::EXIT))
            {
                // go to this screen
                write_line("Exit");
                return "exit";
            }
            if (this->color() == btn_color(Button::OPTS))
            {
                // go to this screen
                write_line("Options");
                return "options";
            }
            return keyinput;
        }

        /**
         * @brief Draws button to screen
         * 
         * @return * void 
         */
        void drawButton()
        {
            draw_sprite(this->m_btn);
        }

        void getButtonImage(std::string image) {};
        void btnImage(std::string image) { }
};

#endif