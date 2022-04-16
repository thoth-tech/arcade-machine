#include "includes.h"

using namespace std;
using std::vector;

int main()
{
    // Load all resources
    load_resource_bundle("bundle", "resources.txt");

    // Instantiate introductory classes
    Helper helper;
    ConfigData configData;
    Splashscreen intro_splashkit("intro_splashkit");

    // Open window and toggle border off.
    open_window("arcade-machine", 1920, 1080);
    window_toggle_border("arcade-machine");

    // Play Thoth Tech intro
    helper.play_intro();

    // Pull the most recent version of the arcade-games repo.
    do
    {
        // Draw SplashKit productions screen
        intro_splashkit.draw_title_page();
        draw_text("Loading...", COLOR_SLATE_GRAY, "btn_font", 60, WIDTH/2 - 100, HEIGHT/2 + 350);
        refresh_screen();
        
    } while (!configData.get_from_git("https://github.com/thoth-tech/arcade-games.git", "games"));
    
    // Get the data from the config files.
    vector<ConfigData> configs = helper.config_data_list();
    //h.GridLayoutExample();
    configData.print_config_data();

    // Grid grid(7, 15);
    // grid.UpdateCell(0, 0, 1, "bgnd");

    Splashscreen s("thoth");
    Menu menu(configs);
    Button *play = new MenuButton(Button::PLAY, 11, 3, 1.2);
    Button *options = new MenuButton(Button::OPTIONS, 11, 4, 1.2);
    Button *exit = new MenuButton(Button::EXIT, 11, 5, 1.2);
    
    s.add_button(play);
    s.add_button(options);
    s.add_button(exit);
    point_2d mousePoint;
    bool playClicked;
    play_music("menu_music");

    while (not quit_requested() && (not key_down(ESCAPE_KEY)))
    {
        process_events();

        clear_screen();

        mousePoint = mouse_position();

        s.draw_title_page();
        s.button_clicked(mousePoint);

        playClicked = s.getPlayClick();

        if (playClicked || (s.get_action() == "play"))
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
