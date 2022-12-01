#ifndef ARCADE_MACHINE_H
#define ARCADE_MACHINE_H

// Arcade Machine Class
#include "Button.h"
#include "MenuButton.h"
#include "GameScreenButton.h"
#include "ButtonNode.h"
#include "ConfigData.h"
#include "GridLayout.h"
#include "Cell.h"
#include "Helper.h"
#include "Menu.h"
#include "Option.h"
#include "Audio.h"
#include "Splashscreen.h"
#include "Selector.h"
#include "splashkit.h"
#include <string>
#include <vector>
#include <fstream>

// Define number of rows and columns in grid
#define ROWS 7
#define COLS 15

/* 
    This class handles the creation of the
    Arcade Machine itself
*/
class ArcadeMachine
{
private:
    /// Instance of Helper
    Helper m_helper;

    /// Instance of ConfigData
    ConfigData m_config;

    /// Vector of ConfigData
    std::vector<ConfigData> m_configs;

    /// Vector of MenuButtons
    std::vector<Button*> m_menuBtns;

    /// Vector of GameScreenButtons
    std::vector<Button*> m_gameBtns;

    /// Thoth Tech Company intro
    Splashscreen m_introThothTech;

    /// Arcade Machine Team intro
    Splashscreen m_introArcadeTeam;

    /// Arcade Machine Team developer names
    std::vector<std::string> m_arcadeTeamDeveloperNames;

    /// SplashKit Production intro
    Splashscreen m_introSplashkit;

    /// SplashKit about screen
    AboutScreen m_aboutScreen;

    /// Main Menu Selector
    Selector m_selectorMainMenu;

    /// Games Menu Selector
    Selector m_selectorGamesMenu;

    /// Instance of Grid Layout
    GridLayout m_grid;

    /// Mouse pointer
    point_2d m_mouse;

    /// Button Action 
    std::string m_action;

    /// Turn menu music on/off
    bool m_playMusic = true;

    ///Music controller
    Audio audio;

    /// Menu button node
    ButtonNode *m_menuButtonNode = nullptr;

    // Helper function to load developer names into m_arcadeTeamDeveloperNames
    // Called in the constructor
    void loadDeveloperNames(const char* filePath);

    /// Check for Options menu exit 
    bool m_exitOptions = false;
public:
    // Default Constructor
    ArcadeMachine();

    // Destructor
    ~ArcadeMachine();

    // Getters
    auto get_configs() const -> const vector<ConfigData>& { return this->m_configs; }

    void mainMenu(); // Starts the Main Menu
    void gamesMenu(); // Starts the Games Menu
    void optionsMenu(); // Starts the Options Menu
    void buttonClicked(point_2d point); // Checks for buttons clicked
    void drawMainMenu();
    void prepareMainMenu();
    void playThothTechIntro();
    void playArcadeTeamIntro();
    void playSplashKitIntro(); // Draws the Splashkit Productions logo to the screen and fetches new games from Git repo
    void printConfigs();
    void exitProgram();

};

#endif