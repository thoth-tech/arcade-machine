using namespace std;

class Splashscreen {
    private:
        vector<Button*> btns;
        bool playClicked = false;
        bool exitClicked = false;
        Selector select;
    public:
        Splashscreen(){}

        void add_button(Button *b)
        {
            this->btns.push_back(b);
        }

        bool getPlayClick()
        {
            return playClicked;
        }

        bool getExitClick()
        {
            return exitClicked;
        }

        void draw_title_page()
        {
            // Draw buttons to screen
            for (int i = 0; i < btns.size(); i++)
            {
                this->btns[i]->draw_button();
            }
            
            // Draw background iamge to screen
            draw_bitmap("bgnd", 0, 0);

            // Draw text on buttons
            draw_text("play!", COLOR_BLACK, "btn_font", 80, btns[0]->x() - (btns[0]->centre_x()/2) + 5, btns[0]->y() - btns[0]->centre_y(), option_to_screen());
            draw_text("options", COLOR_BLACK, "btn_font", 80, btns[1]->x() - (btns[1]->centre_x()/2) - 20, btns[1]->y() - btns[1]->centre_y(), option_to_screen());
            draw_text("exit", COLOR_BLACK, "btn_font", 80, btns[2]->x() - (btns[2]->centre_x()/2) + 20, btns[2]->y() - btns[2]->centre_y(), option_to_screen());

            select.check_key_input(this->btns);
        }

        void button_clicked(point_2d point)
        {
            // If mouse is at button sprite position.
            if (sprite_at(btns[2]->btn(), point))
            {
                // If the mouse is then clicked.
                if (mouse_clicked(LEFT_BUTTON))
                {
                    // Button clicked is true.
                    exitClicked = true;
                    write_line("exit BUTTON CLICKED");
                    exit_program();
                }
            }
            if (sprite_at(btns[0]->btn(), point))
            {
                // If the mouse is then clicked.
                if (mouse_clicked(LEFT_BUTTON))
                {
                    // Button clicked is true.
                    playClicked = true;
                    write_line("Play button clicked");
                }
            }
        }

         void exit_program()
        {
            abort();
        }
};