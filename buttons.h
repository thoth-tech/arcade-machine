// Button Class
#define WIDTH 1920
#define HEIGHT 1080
#define X_CELL 120
#define Y_CELL 135

using namespace std;
using std::vector;


class Button{
    public:
        int _id;
        // This buttons bitmap
        bitmap _pic;
        // This buttons sprite
        sprite _btn;
        // This buttons position in the window as point_2d in pixels (x,y)
        point_2d _location;
        // This buttons position in the window in pixels
        int _x; int _y;
        // This buttons centre point offset width in pixels
        int _centre_x;
        // This buttons centre point offset height in pixels
        int _centre_y;
        // This buttons color 
        string _color;

    
        // Getters:
        auto id() const -> const int& { return _id; }
        auto pic() const -> const bitmap& { return _pic; }
        auto btn() const -> const sprite& { return _btn; }
        auto location() const -> const point_2d& { return _location; }
        auto x() const -> const int& { return _x; }
        auto y() const -> const int& { return _y; }
        auto centre_x() const -> const int& { return _centre_x; }
        auto centre_y() const -> const int& { return _centre_y; }
        auto color() const -> const string& { return _color; }

        // Enumeration of button options
        enum Color
        {
            PLAY,
            EXIT,
            OPTIONS
        };

        // Default Constructor
        Button(){}

        // First Overloaded Constructor
        Button(Color c, float scale = 1)
        {
            // button color
            this->_color = btn_color(c);
            // load button color image
            this->_pic = load_bitmap(this->_color, this->_color);
            // create sprite from image
            this->_btn = create_sprite(this->_pic);
            // scale the sprite
            sprite_set_scale(this->_btn, scale);
        }
        
        // Second Overloaded Constructor
        // Calculates the position in the window
        Button(Color c, float y, float x, float scale = 1)
        {
            // button color
            this->_color = btn_color(c);
            // load button color image
            this->_pic = load_bitmap(this->_color, this->_color);
            // create sprite from image
            this->_btn = create_sprite(this->_pic);
            // get the centre points of the sprite 
            this->_centre_x = sprite_width(this->_btn) /2;
            this->_centre_y = sprite_height(this->_btn)/2;
            // store the intended location 
            this->_x = x * X_CELL; _y = y * Y_CELL;
            // set button sprite centre point to intended location
            sprite_set_x(this->_btn, this->_x - this->_centre_x);
            sprite_set_y(this->_btn, this->_y - this->_centre_y);
            // store the window centre point of button as location
            this->_location = center_point(this->_btn);
            // scale the sprite
            sprite_set_scale(this->_btn, scale);
        }

        ~Button(){}

        virtual void draw_button()
        {
            // draw button to screen
            draw_sprite(this->_btn);
        }

        virtual void action() = 0;

        void draw_screen_guides()
        {
            draw_circle(COLOR_BLACK, WIDTH*0.5 , HEIGHT*0.5 , 3);
            draw_circle(COLOR_BLACK, WIDTH*0.25, HEIGHT*0.5 , 3);
            draw_circle(COLOR_BLACK, WIDTH*0.75, HEIGHT*0.5 , 3);
            draw_circle(COLOR_BLACK, WIDTH*0.5 , HEIGHT*0.25, 3);
            draw_circle(COLOR_BLACK, WIDTH*0.5 , HEIGHT*0.75, 3);                                    
        }

        string btn_color(Color c)
        {
            string filepath = "buttons/Red/";
            switch(c)
            {
                case PLAY:
                    return filepath + "Play.png";
                    break;
                case EXIT:
                    return filepath + "Exit.png";
                    break;
                case OPTIONS:
                    return filepath + "Settings.png";
                    break;
                default:
                    //return;
                    break;
            }
        }
};

class MenuButton : public Button{
    public:
        //MenuButton(){}
        MenuButton(Color c, float y, float x, float scale = 1) : Button(c, y, x, scale){}
        ~MenuButton(){}

        void action()
        {
            if (color() == btn_color(Button::PLAY))
            {
                draw_sprite(btn());
                // go to this screen  
            }
            if (color() == btn_color(Button::EXIT))
            {
                draw_sprite(btn());   
                // go to this screen             
            }
            if (color() == btn_color(Button::OPTIONS))
            {
                draw_sprite(btn());  
                // go to this screen                 
            }
        }
};

class GameScreenButton : public Button{
    public:
        //MenuButton(){}
        GameScreenButton(Color c, float y, float x, float scale = 1) : Button(c, y, x, scale){}
        ~GameScreenButton(){}

        void action()
        {
            // play
        }

};



