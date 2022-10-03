#include "Configuration.h"
#include "ArcadeMachine.h"
#include "wtypes.h"
#include <iostream>
using namespace std;
#define PLAY_INTRO true
#define LOAD_GAMES true

int main(void)
{
    // Load all resources
    set_resources_path("resources" ARCADE_MACHINE_PATH_SEP);
    load_resource_bundle("bundle", "resources.txt");

    // Instantiate Arcade Machine
    ArcadeMachine Arcade;

    // Full screen
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    int horizontal = desktop.right;
    int vertical = desktop.bottom;
    // Open window and toggle border off.
    open_window("arcade-machine", horizontal, vertical);
    window_toggle_border("arcade-machine");

#if PLAY_INTRO == true
    // Play Thoth Tech intro
    Arcade.playThothTechIntro();
    Arcade.playArcadeTeamIntro();
#endif

#if LOAD_GAMES == true
    // Play SplashKit intro
    Arcade.playSplashKitIntro();
#endif
    
    // Prepare the main menu
    Arcade.prepareMainMenu();
    // Draw the main menu
    Arcade.mainMenu();

    // ISSUE - we never get here because the program exits forcibly from ArcadeMachine, rather than exiting from main and returning 0.
    std::cout << "we never reach this point to output?\n";

    free_resource_bundle("bundle");

    return 0;
}