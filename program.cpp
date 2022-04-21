#include "includes.h"

using namespace std;
using std::vector;

int main()
{
    // Load all resources
    load_resource_bundle("bundle", "resources.txt");

    // Instantiate introductory classes
    ArcadeMachine Arcade;
    ConfigData configData;
    Splashscreen intro_splashkit("intro_splashkit");

    // Open window and toggle border off.
    open_window("arcade-machine", 1920, 1080);
    window_toggle_border("arcade-machine");

    bool play_intro = true;
    bool load_games = true;

    // Play Thoth Tech intro
    if (play_intro)
        Arcade.play_intro();
    // Play SplashKit intro
    if (load_games)
        Arcade.load_games();
    
    // Initialise grid 
    Grid grid(7, 15);
    //Splashscreen s("thoth");
    Menu menu(Arcade.get_configs());
    // Button *play = new MenuButton(Button::PLAY, 11, 3, 1.2);
    // Button *options = new MenuButton(Button::OPTIONS, 11, 4, 1.2);
    // Button *exit = new MenuButton(Button::EXIT, 11, 5, 1.2);
    Button *play = new MenuButton(Button::PLAY, 1.2);
    Button *options = new MenuButton(Button::OPTIONS, 1.2);
    Button *exit = new MenuButton(Button::EXIT, 1.2);
    string image = path_to_resource("thoth", IMAGE_RESOURCE);
    bitmap thoth = load_bitmap("thoth", image);

    grid.UpdateCell(thoth, 0, 0, 1, false);
    grid.UpdateCell(play, 2, 10);
    grid.UpdateCell(options, 3, 10);
    grid.UpdateCell(exit, 4, 10);
    
    // s.add_button(play);
    // s.add_button(options);
    // s.add_button(exit);
    point_2d mousePoint;
    bool playClicked = false;
    // Play main menu music
    //play_music("menu_music");

    while (not quit_requested() && (not key_down(ESCAPE_KEY)))
    {
        process_events();
        clear_screen();
        // Get mouse position
        mousePoint = mouse_position();

        grid.DrawGrid();

        //s.draw_title_page();
        //s.button_clicked(mousePoint);

        //playClicked = s.getPlayClick();

        if (playClicked )//|| (s.get_action() == "play"))
        {
            fade_music_out(1000);

            while (not quit_requested() && (not key_down(ESCAPE_KEY)))
            {
                process_events();
                clear_screen();
                // Draw the menu page.
                menu.draw_menu_page();
                // Listen for button click and get mouse location.
                menu.button_clicked(mousePoint);
                // Keep this running while game is played to keep mouse in the game window.
                menu.move_mouse_position(mousePoint);
                refresh_screen(60);
            }
        }
        refresh_screen(60);
    }
    return 0;
}
