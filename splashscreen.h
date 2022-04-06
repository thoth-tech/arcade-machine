using namespace std;

class Splashscreen {
    private:
        sprite background = create_sprite("background.png");
        sprite playButton = create_sprite("red_button01.png");
        sprite exitButton = create_sprite("yellow_button04.png");
        bool playClicked = false;
        bool exitClicked = false;

    public:
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
            draw_sprite(background, 0, 0);

            point_2d playPosition;
            playPosition.x = 820;
            playPosition.y = 620;

            sprite_set_position(playButton, playPosition);
            draw_sprite(playButton, 0, 0);

            point_2d exitPosition;
            exitPosition.x = 820;
            exitPosition.y = 820;

            sprite_set_position(exitButton, exitPosition);
            draw_sprite(exitButton, 0, 0);
        }

        void button_clicked(point_2d point)
        {
            // If mouse is at button sprite position.
            if (sprite_at(exitButton, point))
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
            if (sprite_at(playButton, point))
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