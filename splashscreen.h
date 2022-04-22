using namespace std;

class Splashscreen {
    private:
        // Vector of buttons
        vector<Button*> btns;
        // Bitmap to be drawn as background
        string bmp;
        // Check if play has been clicked
        bool playClicked = false;
        // CHeck if exit has been clicked
        bool exitClicked = false;
        // A selector
        Selector select;
        // The action to take
        string action;
    public:
        // Default constructor
        Splashscreen(){}
        // Overloaded constructor
        Splashscreen(string bitmap)
        {
            this->bmp = bitmap;
        }

        void add_button(Button *b){ this->btns.push_back(b); }
        
        bool getPlayClick(){ return this->playClicked; }
        bool getExitClick(){ return this->exitClicked; }

        string get_action(){ return this->action; }

        void draw_title_page()
        {            
            // Draw background image to screen
            draw_bitmap(this->bmp, 0, 0);

            // if (this->btns.size() > 0)
            // {
            //     // Arcade Machine title
            //     draw_text("Arcade",  COLOR_BLACK, "title_font", 100, 1160, 100);
            //     draw_text("Machine", COLOR_BLACK, "title_font", 100, 1130, 200);
                
            //     // Draw buttons to screen
            //     for (int i = 0; i < btns.size(); i++)
            //     {
            //         this->btns[i]->draw_button();
            //     }

            //     // Draw cursor
            //     draw_sprite(select.get_cursor());

            //     // Draw text on buttons
            //     draw_text("play!", COLOR_BLACK, "btn_font", 70, btns[0]->x() - (btns[0]->centre_x()/2) + 5, btns[0]->y() - btns[0]->centre_y()+5);
            //     draw_text("options", COLOR_BLACK, "btn_font", 70, btns[1]->x() - (btns[1]->centre_x()/2) - 20, btns[1]->y() - btns[1]->centre_y()+5);
            //     draw_text("exit", COLOR_BLACK, "btn_font", 70, btns[2]->x() - (btns[2]->centre_x()/2) + 20, btns[2]->y() - btns[2]->centre_y()+5);

            //     this->action = select.check_key_input(this->btns);
                
            //     // Draw creators
            //     draw_text("Created By", COLOR_BLACK, "roboto", 14, 1200, 850);
            //     draw_text("Sarah", COLOR_BLACK, "roboto", 14, 1200, 870);
            //     draw_text("Anthony", COLOR_BLACK, "roboto", 14, 1200, 890);
            //     draw_text("Riley", COLOR_BLACK,  "roboto", 14, 1200, 910);
            //     draw_text("Huy", COLOR_BLACK, "roboto", 14, 1200, 930);
            // }
        }

        void button_clicked(point_2d point)
        {
            // Play
            if ( action == "play" || (sprite_at(btns[0]->btn(), point) && mouse_clicked(LEFT_BUTTON)) )
            {
                playClicked = true;
                write_line("Play button clicked");
            }

            // Options
            if ( action == "options" || (sprite_at(btns[1]->btn(), point) && mouse_clicked(LEFT_BUTTON)) )
            {
                write_line("Options button clicked");
            }

            // Exit
            if ( action == "exit" || (sprite_at(btns[2]->btn(), point) && mouse_clicked(LEFT_BUTTON)) )
            {
                exitClicked = true;
                write_line("Exit button clicked");
                exit_program();
            }
        }

         void exit_program()
        {
            abort();
        }
};