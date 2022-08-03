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
class Button{
    public:
        /// This buttons ID
        int _id;
        /// This buttons bitmap
        bitmap _pic;
        /// This buttons sprite
        sprite _btn;
        /// This buttons position in the window as point_2d in pixels (x,y)
        point_2d _location;
        /// This buttons position in the window in pixels
        int _x; int _y;
        /// This buttons centre point offset width in pixels
        int _centre_x;
        /// This buttons centre point offset height in pixels
        int _centre_y;
        /// This buttons color 
        std::string _color;
        /// This buttons hightlight bitmap
        bitmap _hghlt;
        /// This buttons hightlight layer
        std::string _highlight;
        /// This buttons image path
        std::string _image_path;
    
        /// Getters:
        auto id()       const -> const int&      { return _id;       }
        auto pic()      const -> const bitmap&   { return _pic;      }
        auto btn()      const -> const sprite&   { return _btn;      }
        auto location() const -> const point_2d& { return _location; }
        auto x()        const -> const int&      { return _x;        }
        auto y()        const -> const int&      { return _y;        }
        auto centre_x() const -> const int&      { return _centre_x; }
        auto centre_y() const -> const int&      { return _centre_y; }
        auto color()    const -> const std::string&   { return _color;    }

        /// Setters
        void set_id(int id)     { this->_id = id; }
        void set_x(const int &x){ this->_x  = x;  }
        void set_y(const int &y){ this->_y  = y;  }

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
            this->_color = btn_color(c);
            // load button color image
            //this->_pic = load_bitmap(to_string(c), this->_color);
            // create sprite from image
            this->_btn = create_sprite(this->_color);
            // add hightlight layer to sprite
            sprite_add_layer(this->_btn, this->_hghlt, this->_highlight);
            // scale the sprite
            sprite_set_scale(this->_btn, scale);
            // get the centre points of the sprite 
            this->_centre_x = sprite_width(this->_btn) /2;
            this->_centre_y = sprite_height(this->_btn)/2;
        }
        
        /**
         * @brief Second Overloaded Constructor
         * Calculates the position in the window
         * 
         * @param c button type / color
         * @param x intended x-axis position in window
         * @param y intended y-axis position in window
         * @param x_cell size of cell x-dimension 
         * @param y_cell size of cell y-dimension
         * @param scale size multiplier
         */
        Button(Color c, float x, float y, int x_cell, int y_cell, float scale = 1)
        { 
            // button color
            this->_color = btn_color(c);
            // create sprite from image
            this->_btn = create_sprite(this->_color);
            // add hightlight layer to sprite
            sprite_add_layer(this->_btn, this->_hghlt, this->_highlight);
            // get the centre points of the sprite 
            this->_centre_x = sprite_width(this->_btn) /2;
            this->_centre_y = sprite_height(this->_btn)/2;
            // store the intended location 
            this->_x = x * x_cell; _y = y * y_cell;
            // set button sprite centre point to intended location
            sprite_set_x(this->_btn, this->_x - this->_centre_x);
            sprite_set_y(this->_btn, this->_y - this->_centre_y);
            // store the window centre point of button as location
            this->_location = center_point(this->_btn);
            // scale the sprite
            sprite_set_scale(this->_btn, scale);
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
            this->_color = btn_color(c);
            // create bitmap from string
            //this->_pic = load_bitmap(image, image);
            // create sprite from image
            this->_btn = create_sprite(image);
            // add hightlight layer to sprite
            sprite_add_layer(this->_btn, this->_hghlt, this->_highlight);
            // scale the sprite
            sprite_set_scale(this->_btn, scale);
            // get the centre points of the sprite 
            this->_centre_x = sprite_width(this->_btn) /2;
            this->_centre_y = sprite_height(this->_btn)/2;
        }

        // Destructor
        ~Button(){}

        // Virtual fucntions
        virtual void btn_image(std::string image) = 0;
        virtual void draw_button() = 0;
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
                    _hghlt = bitmap_named("play_hghlt");
                    _highlight = "play_highlight";
                    return "btn_play";
                    break;
                case EXIT:
                    _hghlt = bitmap_named("exit_hghlt");
                    _highlight = "exit_highlight";
                    return "btn_exit";
                    break;
                case OPTS:
                    _hghlt = bitmap_named("options_hghlt");
                    _highlight = "options_highlight";
                    return "btn_opts";
                    break;
                case GAME:
                    _hghlt = bitmap_named("game_hghlt");
                    _highlight = "game_highlight";
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
class MenuButton : public Button{
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
        void draw_button()
        {
            draw_sprite(this->_btn);
        }

        void get_button_image(std::string image) {};
        void btn_image(std::string image) { }
};

/**
 * @brief Buttons created for Game Screen Menu
 * 
 * Derived from abstract Button class
 */
class GameScreenButton : public Button{
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

        void btn_image(std::string image)
        {
            this->_color = image;
        }

        // draw button to screen
        void draw_button()
        {
            draw_sprite(this->_btn);
        }
};

#endif