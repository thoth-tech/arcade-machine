#include "MenuButton.h"

// First constructor
MenuButton::MenuButton(Color c, float scale)
     : Button(c, scale)
{

}

/**
* @brief The action of this button
* Called when the selector receives input for this button
* 
* @param keyinput 
* @return string
*/
std::string MenuButton::action(std::string keyinput)
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
void MenuButton::drawButton()
{
    draw_sprite(this->m_btn);
}