#include "Button.h"

/**
* @brief First Overloaded Constructor
* 
* @param c button type / color
* @param scale size multiplier
* 
*/
Button::Button(Color c, float scale)
{
    // button color
    this->m_btnColor = btn_color(c);
    // load button color image
    //this->_pic = load_bitmap(to_string(c), this->_color);
    // create sprite from image
    this->m_btn = create_sprite(this->m_btnColor);
    // add hightlight layer to sprite
    sprite_add_layer(this->m_btn, this->m_btnHighlightPic, this->m_btnHighlightText);
    // scale the sprite
    sprite_set_scale(this->m_btn, scale);
    // get the centre points of the sprite 
    this->m_centreX = sprite_width(this->m_btn) /2;
    this->m_centreY = sprite_height(this->m_btn)/2;
}
        
/**
* @brief Second Overloaded Constructor
* Calculates the position in the window
* 
* @param c button type / color
* @param x intended x-axis position in window
* @param y intended y-axis position in window
* @param xCell size of cell x-dimension 
* @param yCell size of cell y-dimension
* @param scale size multiplier
*/
Button::Button(Color c, float x, float y, int xCell, int yCell, float scale)
{ 
    // button color
    this->m_btnColor = btn_color(c);
    // create sprite from image
    this->m_btn = create_sprite(this->m_btnColor);
    // add hightlight layer to sprite
    sprite_add_layer(this->m_btn, this->m_btnHighlightPic, this->m_btnHighlightText);
    // get the centre points of the sprite 
    this->m_centreX = sprite_width(this->m_btn) /2;
    this->m_centreY = sprite_height(this->m_btn)/2;
    // store the intended location 
    this->m_x = x * xCell; m_y = y * yCell;
    // set button sprite centre point to intended location
    sprite_set_x(this->m_btn, this->m_x - this->m_centreX);
    sprite_set_y(this->m_btn, this->m_y - this->m_centreY);
    // store the window centre point of button as location
    this->m_btnLocation = center_point(this->m_btn);
    // scale the sprite
    sprite_set_scale(this->m_btn, scale);
}

/**
* @brief Third Overloaded Constructor
* Calculates the position in the window
* 
* @param c button type / color
* @param image bitmap name
* @param scale size multiplier
*/
Button::Button(Color c, std::string image, float scale)
{
    // button color
    this->m_btnColor = btn_color(c);
    // create bitmap from string
    //this->_pic = load_bitmap(image, image);
    // create sprite from image
    this->m_btn = create_sprite(image);
    // add hightlight layer to sprite
    sprite_add_layer(this->m_btn, this->m_btnHighlightPic, this->m_btnHighlightText);
    // scale the sprite
    sprite_set_scale(this->m_btn, scale);
    // get the centre points of the sprite 
    this->m_centreX = sprite_width(this->m_btn) /2;
    this->m_centreY = sprite_height(this->m_btn)/2;
}

/**
* @brief Gets the filepath to the requested color (image) 
* 
* @param c 
* @return * string 
*/
std::string Button::btn_color(Color c)
{
    std::string filepath = "buttons/";
    switch(c)
    {
        case PLAY:
            m_btnHighlightPic = bitmap_named("play_hghlt");
            m_btnHighlightText = "play_highlight";
            return "btn_play";
            break;
        case EXIT:
            m_btnHighlightPic = bitmap_named("exit_hghlt");
            m_btnHighlightText = "exit_highlight";
            return "btn_exit";
            break;
        case OPTS:
            m_btnHighlightPic = bitmap_named("options_hghlt");
            m_btnHighlightText = "options_highlight";
            return "btn_opts";
            break;
        case GAME:
            m_btnHighlightPic = bitmap_named("game_hghlt");
            m_btnHighlightText = "game_highlight";
            return "";
        case HOME:
            return filepath + "Gray/home.png";
            break;
        default:
            return "btn_play";
            break;
    }
}

