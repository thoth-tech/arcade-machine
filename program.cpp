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
    if (play_intro) Arcade.play_intro();
    // Play SplashKit intro
    if (load_games) Arcade.load_games();
    
    // Prepare the main menu
    Arcade.prepare_main_menu();
    // Draw the main menu
    Arcade.main_menu();
    // Draw games menu
    Arcade.games_menu();

    return 0;
}
