#ifndef ARCADE_MACHINE_MENU_H
#define ARCADE_MACHINE_MENU_H

#include "ConfigData.h"
#include "Tip.h"
#include "Selector.h"
#include "GameData.h"
#include "Database.h"
#include "Rating.h"
#include "Table.h"
#include "GridLayout.h"
#include "Button.h"
#include "MenuButton.h"
#include "GameScreenButton.h"
#include "Configuration.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <string>
#include <vector>
#include <chrono>

class Menu {
private:
    std::string m_background = "games_dashboard";
    // Vector to store the config data of each game
    std::vector<ConfigData> m_games;
    Database *m_db;
    Rating m_rating;
    GameData m_gameData;

#ifdef _WIN32
    // Contains info about newly created process and thread
    PROCESS_INFORMATION m_processInfo;
    // Unsigned int to store exit info
    DWORD m_exitCode;
    // Holds the game path of selected game
    LPCSTR m_gamePath;
    // Holds the executable of selected game
    LPSTR m_gameExe;
    // Holds the game directory of selected game
    LPCSTR m_gameDir;
    // m_handle for game window.
    HWND m_handle;
#else
    int m_processId;
#endif

    // Used to find x centre of screen
    double m_centreX = ARCADE_MACHINE_RES_X / 2;
    // Used to fine y centre of screen
    double m_centreY = ARCADE_MACHINE_RES_Y / 2;
    // Stores x of mouse position
    double m_x;
    // Stores y of mouse position
    double m_y;
    // Checks if game is running
    bool m_inGame = false;
    // Checks if program has exited
    bool m_programExit;
    // Vector of buttons
    std::vector<Button*> m_btns;
    // Vector to store game images
    std::vector<std::string> m_gameImages;
    // Menu grid
    GridLayout m_grid;
    Tip *m_tip;
    ButtonNode *m_button = nullptr;
    bool m_overlayActive = false;
    /// Button Action
    std::string m_action;

    Selector m_selectorGamesMenu;

    // Passes into Selector optional parameter.
    bool m_gameMenu = true;

    // Determines when game has started.
    bool m_gameStarted = false;
    // Starting position of button x.
    const int m_posX = 700;
    // Position of button y.
    const int m_posY = 270;
    // Increments x of sprite.
    const int m_speed = 30;
    // The positions of the sprites for slide.
    int m_pos1 = m_posX;
    int m_pos2 = m_pos1 - m_posX;
    int m_pos3 = m_pos2 - m_posX;
    int m_pos4 = m_posX * 2;
    int m_pos5 = m_posX * 3;
    // Hold the button sprites for slide.
    sprite m_newButton1;
    sprite m_newButton2;
    sprite m_newButton3;
    // Determines when sliding.
    bool m_menuSliding = false;

public:
    Menu();
    Menu(std::vector<ConfigData> configs);
    ~Menu();

    // Getters
    auto getButtons() const -> const std::vector<Button*> { return this->m_btns; }
    bool getOverlayState() { return m_overlayActive; }

    std::vector<std::string> getGameSprites(std::vector<ConfigData> configs); // gets game images from the config files and returns vector

    void createGrid(); // Create a GridLayout object
    void createButtons(); // Create a list of games
    void createTip(); // create a tip to display to the user.
    void updateCarousel();
    void carouselHandler();
    void drawMenuPage();
    void updateSlide(sprite buttonSprite, int position); // Method to update the sprite positions and draw sprite.
    void drawUpdateSlideLeft(); // Slide the game buttons on left key input.
    void drawUpdateSlideRight(); // Slide the game buttons on right key input.
    void drawOverlay(ConfigData config, GameData stats); // Draw an overlay over the game, using data from the config.

#ifdef _WIN32
    bool focusWindow(std::string windowName, int timeout = 2000);
    void startGame(LPCSTR gamePath, LPSTR gameExe, LPCSTR gameDirectory);
#else
    void startGame(struct s_ExecutablePath path);
#endif
    void checkGameExit();

    void backToGamesMenu(); // Fade back to games menu
    void fade(double alphaStart, double alphaEnd, double alphaStep);

};

#endif