#include "ArcadeMachine.h"

using namespace std;

int main()
{
    // Load all resources
    load_resource_bundle("bundle", "resources.txt");

    // Instantiate Arcade Machine
    ArcadeMachine Arcade;

    // Open window and toggle border off.
    open_window("arcade-machine", WIDTH, HEIGHT);
    window_toggle_border("arcade-machine");

    // Do you want to play the intros and fetch new games? 
    bool play_intro = true;
    bool load_games = true;

    // Play Thoth Tech intro
    if (play_intro) 
    {
        Arcade.intro_animation(Arcade.get_intro_thoth_tech(), "intro_thoth");
        Arcade.intro_animation(Arcade.get_intro_arcade_team(), "intro_coin", "intro_start");
    }
    // Play SplashKit intro
    if (load_games) Arcade.intro_splashkit();
    
    // Prepare the main menu
    Arcade.prepare_main_menu();
    // Draw the main menu
    Arcade.main_menu();

    return 0;
}
