#include "ArcadeMachine.h"

int main()
{
    // Load all resources
    set_resources_path("resources" ARCADE_MACHINE_PATH_SEP);
    load_resource_bundle("bundle", "resources.txt");

    // Instantiate Arcade Machine
    ArcadeMachine Arcade;

    // Open window and toggle border off.
    open_window("arcade-machine", ARCADE_MACHINE_RES_X, ARCADE_MACHINE_RES_Y);
    window_toggle_border("arcade-machine");

    // Do you want to play the intros and fetch new games? 
    bool play_intro = true;
    bool load_games = true;

    // Play Thoth Tech intro
    if (play_intro) 
    {
        Arcade.playThothTechIntro();
        Arcade.playArcadeTeamIntro();
    }

    // Play SplashKit intro
    if (load_games) Arcade.playSplashKitIntro();
    
    // Prepare the main menu
    Arcade.prepareMainMenu();
    // Draw the main menu
    Arcade.mainMenu();

    free_resource_bundle("bundle");

    return 0;
}