using namespace std;

class Splashscreen {
    private:
        string bmp;
    public:
        // Default constructor
        Splashscreen(){}
        // Overloaded constructor
        Splashscreen(string bitmap){ this->bmp = bitmap; }
        // Draw background image to screen
        void draw_title_page() { draw_bitmap(this->bmp, 0, 0); }
};