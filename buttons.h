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
        // This buttons hightlight bitmap
        bitmap _hghlt;
        // This buttons hightlight layer
        string _hightlight;
        // This buttons image path
        string _image_path;
    
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
            OPTIONS, 
            GAME
        };

        // Default Constructor
        Button(){}

        // First Overloaded Constructor
        Button(Color c, float scale = 1)
        {
            // button color
            this->_color = btn_color(c);
            // load button color image
            this->_pic = load_bitmap(to_string(c), this->_color);
            // create sprite from image
            this->_btn = create_sprite(this->_pic);
            // add hightlight layer to sprite
            sprite_add_layer(this->_btn, this->_hghlt, this->_hightlight);
            // scale the sprite
            sprite_set_scale(this->_btn, scale);
        }
        
        // Second Overloaded Constructor
        // Calculates the position in the window
        Button(Color c, float x, float y, float scale = 1)
        {
            // button color
            this->_color = btn_color(c);
            // load button color image
            this->_pic = load_bitmap(to_string(c), this->_color);
            // create sprite from image
            this->_btn = create_sprite(this->_pic);
            // add hightlight layer to sprite
            sprite_add_layer(this->_btn, this->_hghlt, this->_hightlight);
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

        // Third Overloaded Constructor
        Button(Color c, string image, float scale = 1)
        {
            // button color
            this->_color = btn_color(c);
            // create sprite from image
            this->_btn = create_sprite(image);
            // add hightlight layer to sprite
            sprite_add_layer(this->_btn, this->_hghlt, this->_hightlight);
            // scale the sprite
            sprite_set_scale(this->_btn, scale);
        }

        ~Button(){}

        virtual void btn_image(string image) = 0;
        virtual void draw_button() = 0;
        virtual string action() = 0;
        

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
            string filepath = "buttons/";
            switch(c)
            {
                case PLAY:
                    _hghlt = load_bitmap("play_hghlt", filepath + "btn_play_hghlt.png");
                    _hightlight = "play_hightlight";
                    return filepath + "btn_play.png";
                    break;
                case EXIT:
                    _hghlt = load_bitmap("exit_hghlt", filepath + "btn_exit_hghlt.png");
                    _hightlight = "exit_hightlight";
                    return filepath + "btn_exit.png";
                    break;
                case OPTIONS:
                    _hghlt = load_bitmap("options_hghlt", filepath + "btn_options_hghlt.png");
                    _hightlight = "options_hightlight";
                    return filepath + "btn_options.png";
                    break;
                case GAME:
                    _hghlt = load_bitmap("game_hghlt", filepath + "btn_game_hghlt.png");
                    _hightlight = "game_hightlight";
                    return "";
                default:
                    return filepath + "btn_play.png";
                    break;
            }
        }


};

class MenuButton : public Button{
    public:
        //MenuButton(){}
        MenuButton(Color c, float x, float y, float scale = 1) : Button(c, x, y, scale){}
        ~MenuButton(){}

        string action()
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
            if (this->color() == btn_color(Button::OPTIONS))
            {
                // go to this screen
                write_line("Options");
                return "options";
            }
            return "";
        }

        void draw_button()
        {
            // draw button to screen
            draw_sprite(this->_btn);
        }

        void get_button_image(string image) {};
        void btn_image(string image) { }
};

class GameScreenButton : public Button{
    private: 

    public:
        GameScreenButton(Color c, string image, float scale = 1) : Button(c, image, scale){}
        ~GameScreenButton(){}

        string action()
        {
            return "";
        }

        void btn_image(string image)
        {
            this->_color = image;
        }

        void draw_button()
        {
            // draw button to screen
            draw_sprite(this->_btn);
        }
};



