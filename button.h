// Button Class
#define WIDTH 1920
#define HEIGHT 1080
#define X_CELL 120
#define Y_CELL 135

using namespace std;
using std::vector;


class Button{
    private:
        bitmap _pic;
        sprite _btn;
        point_2d _location;
        int _x; int _y;
        int _w; int _h;
        int _centre_x;
        int _centre_y;
        string color;

    public:
        enum Color
        {
            BLACK,
            BLUE,
            BROWN,
            DARK_GREEN,
            DARK_RED,
            GREEN,
            GREY,
            LIGHT_BLUE,
            ORANGE,
            RED,
            WHITE,
            YELLOW
        };

        // Default Constructor
        Button(){}
        ~Button(){}
        
        // Overloaded Constructor
        Button(Color c, float x, float y)
        {
            // button color
            color = btn_color(c);

            // load button color image
            _pic = load_bitmap(color, color);
            //write_line(color);

            // create sprite from image
            _btn = create_sprite(_pic);

            // get the centre points of the sprite 
            _centre_x = sprite_width(_btn)/2;
            _centre_y = sprite_height(_btn)/2;

            // store the intended location 
            _x = x*X_CELL; _y = y*Y_CELL;
            //write_line("x: "+to_string(_x)+" y: "+to_string(_y));

            // set button sprite centre point to intended location
            sprite_set_x(_btn, _x - _centre_x);
            sprite_set_y(_btn, _y - _centre_y);

            // store the window centre point of button as location
            _location = center_point(_btn);

            
            sprite_set_scale(_btn, 1.5);

            // draw button to screen
            draw_sprite(_btn);
        }

        
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
                case BLACK:
                    return filepath + "btn_black.png";
                    break;
                case BLUE:
                    return filepath + "btn_blue.png";
                    break;
                case BROWN:
                    return filepath + "btn_brown.png";
                    break;
                case DARK_GREEN:
                    return filepath + "btn_dark_green.png";
                    break;
                case DARK_RED:
                    return filepath + "btn_dark_red.png";
                    break;
                case GREEN:
                    return filepath + "btn_green.png";
                    break;
                case GREY:
                    return filepath + "btn_grey.png";
                    break;
                case LIGHT_BLUE:
                    return filepath + "btn_light_blue.png";
                    break;
                case ORANGE:
                    return filepath + "btn_orange.png";
                    break;
                case RED:
                    return filepath + "btn_red.png";
                    break;
                case WHITE:
                    return filepath + "btn_white.png";
                    break;
                case YELLOW:
                    return filepath + "btn_yellow.png";
                    break;
                default:
                    return filepath + "btn_light_blue.png";
                    break;
            }
        }
};



