#include "OptionsScreenButton.h"

// First constructor
OptionsScreenButton::OptionsScreenButton(Color c, float scale) : Button(c, scale)
{
}

/**
 * @brief returns the action
 *
 * @param keyinput
 * @return string
 */
std::string OptionsScreenButton::action(std::string keyinput)
{
    if (this->color() == btn_color(Button::HOME)) {
        // go to this screen
        write_line("Home");
        return "home";
    }
    if (this->color() == btn_color(Button::SOUND)) {
        // go to this screen
        write_line("Sound");
        return "sound";
    }
    if (this->color() == btn_color(Button::DISPLAY)) {
        // go to this screen
        write_line("Display");
        return "display";
    }
    if (this->color() == btn_color(Button::STATS)) {
        // go to this screen
        write_line("Stats");
        return "stats";
    }

    return keyinput;
}

void OptionsScreenButton::btnImage(std::string image)
{
    this->m_btnColor = image;
}

// draw button to screen
void OptionsScreenButton::drawButton()
{
    draw_sprite(this->m_btn);
}