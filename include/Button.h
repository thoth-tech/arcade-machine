#ifndef ARCADE_MACHINE_BUTTON_H
#define ARCADE_MACHINE_BUTTON_H

#include <string>
#include "splashkit.h"

/**
 * @brief Abstract Button Class
 * 
 * This abstract class is the base class for all derived classes
 * Contains three overloaded contructors and three virtual methods 
 */
class Button {
    public:
        /// This buttons ID
        int m_id;
        /// This buttons bitmap
        bitmap m_pic;
        /// This buttons sprite
        sprite m_btn;
        /// This buttons position in the window as point_2d in pixels (x,y)
        point_2d m_btnLocation;
        /// This buttons position in the window in pixels
        int m_x; 
        int m_y;
        /// This buttons centre point offset width in pixels
        int m_centreX;
        /// This buttons centre point offset height in pixels
        int m_centreY;
        std::string m_btnColor;
        /// This buttons hightlight bitmap
        bitmap m_btnHighlightPic;
        /// This buttons hightlight layer
        std::string m_btnHighlightText;
        /// This buttons image path
        std::string m_imagePath;
    
        /// Getters:
        auto id()       const -> const int&      { return m_id;       }
        auto pic()      const -> const bitmap&   { return m_pic;      }
        auto btn()      const -> const sprite&   { return m_btn;      }
        auto location() const -> const point_2d& { return m_btnLocation; }
        auto x()        const -> const int&      { return m_x;        }
        auto y()        const -> const int&      { return m_y;        }
        auto centreX() const -> const int&      { return m_centreX; }
        auto centreY() const -> const int&      { return m_centreY; }
        auto color()    const -> const std::string&   { return m_btnColor;    }

        /// Setters
        void setId(int id)     { this->m_id = id; }
        void setX(const int &x){ this->m_x  = x;  }
        void setY(const int &y){ this->m_y  = y;  }

        /**
         * @brief Enumeration of button types
         * 
         */
        enum Color
        {
            PLAY,
            EXIT,
            OPTS, 
            GAME,
            HOME
        };

        // Default Constructor
        Button(){}

        /**
         * @brief First Overloaded Constructor
         * 
         * @param c button type / color
         * @param scale size multiplier
         * 
         */
        Button(Color c, float scale = 1)
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
        Button(Color c, float x, float y, int xCell, int yCell, float scale = 1)
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
        Button(Color c, std::string image, float scale = 1)
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

        // Destructor
        ~Button(){}

        // Virtual fucntions
        virtual void btnImage(std::string image) = 0;
        virtual void drawButton() = 0;
        virtual std::string action(std::string keyinput = "") = 0;

        /**
         * @brief Gets the filepath to the requested color (image) 
         * 
         * @param c 
         * @return * string 
         */
        std::string btn_color(Color c)
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
};

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

/**
 * @brief Buttons created for Game Screen Menu
 * 
 * Derived from abstract Button class
 */
class GameScreenButton : public Button {
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