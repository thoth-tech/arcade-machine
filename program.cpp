#include "includes.h"

using namespace std;

// Define screen size (in pixels)
//#define WIDTH  1920
//#define HEIGHT 1080

int main()
{
    // Load all resources
    load_resource_bundle("bundle", "resources.txt");

    // Instantiate Arcade Machine
    ArcadeMachine Arcade;

    // Open window and toggle border off.
    open_window("arcade-machine", WIDTH, HEIGHT);
    window_toggle_border("arcade-machine");

    // Do we want to play the intro?
    // Do we want to fetch new games? 
    bool play_intro = false;
    bool load_games = false;

    // Play Thoth Tech intro
    if (play_intro)
        Arcade.play_intro();
    // Play SplashKit intro
    if (load_games)
        Arcade.load_games();
    
    // Prepare the Main Menu
    Arcade.prepare_main_menu();

    while (!quit_requested() && (!key_down(ESCAPE_KEY)))
    {
        process_events();
        clear_screen();
        
        // Draw the Main Menu
        Arcade.draw_main_menu();

        //playClicked = s.getPlayClick();

        // if (play_clicked)//|| (s.get_action() == "play"))
        // {
        //     fade_music_out(1000);

        //     while (not quit_requested() && (not key_down(ESCAPE_KEY)))
        //     {
        //         process_events();
        //         clear_screen();
        //         // Draw the menu page.
        //         //menu.draw_menu_page();
        //         // Listen for button click and get mouse location.
        //         //menu.button_clicked(mousePoint);
        //         // Keep this running while game is played to keep mouse in the game window.
        //         //menu.move_mouse_position(mousePoint);
        //         refresh_screen(60);
        //     }
        // }
        refresh_screen(60);
    }
    return 0;
}
