#include "includes/GameScreenButton.h"

// First constructor
GameScreenButton::GameScreenButton(Color c, float scale)
     : Button(c, scale)
{

}

// Third constructor
GameScreenButton::GameScreenButton(Color c, std::string s, float scale)
     : Button(c, s, scale)
{

}

/**
* @brief returns the action 
* 
* @param keyinput 
* @return string 
*/
std::string GameScreenButton::action(std::string keyinput)
{
    return keyinput;
}

void GameScreenButton::btnImage(std::string image)
{
    this->m_btnColor = image;
}

// draw button to screen
void GameScreenButton::drawButton()
{
    draw_sprite(this->m_btn);
}