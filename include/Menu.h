#ifndef ARCADE_MACHINE_MENU_H
#define ARCADE_MACHINE_MENU_H

#include "Tip.h"
#include "Selector.h"
#include "Button.h"
#include "MenuButton.h"

#ifdef _WIN32
#include <Windows.h>
#endif

class Menu {
private:
    std::string m_background = "games_dashboard";
    // Vector to store the config data of each game
    std::vector<ConfigData> m_games;

#ifdef _WIN32
    // Contains info about newly created process and thread
    PROCESS_INFORMATION processInfo;
    // Unsigned int to store exit info
    DWORD exit_code;
    // Holds the game path of selected game
    LPCSTR _gamePath;
    // Holds the executable of selected game
    LPSTR _gameExe;
    // Holds the game directory of selected game
    LPCSTR _gameDir;
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
    // Vectore to store game images
    std::vector<std::string> m_gameImages;
    // Menu grid
    Grid m_grid;
    Tip *m_tip;
    ButtonNode *m_button = nullptr;
    bool _overlayActive = false;
    /// Button Action
    std::string _action;
    Selector m_selectorGamesMenu;
    // Passes into Selector optional parameter.
    bool m_gameMenu = true;

#ifdef _WIN32
    // Handle for game window.
    HWND handle;
#endif

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
    Menu(){}

    Menu(std::vector<ConfigData> configs)
    {
        this->m_games = configs;

#ifdef _WIN32
        handle = FindWindowA(NULL, "arcade-machine");
#endif
    }
    ~Menu(){}

    // Getters
    auto getButtons() const -> const std::vector<Button*> { return this->m_btns; }
    bool getOverlayState() { return _overlayActive; }

    /** 
     * @brief Gets the game images from the config files and returns vector of game images.
     * 
     * @param configs Vector of config data.
     * @return vector of game images.
     */
    std::vector<std::string> getGameSprites(std::vector<ConfigData> configs)
    {
        std::vector<std::string> gameImages;

        for (int i = 0; i < configs.size(); i++)
        {
            // Get image dir and image name from games vector.
            std::string image = configs[i].folder() + "/" + configs[i].image();
            gameImages.push_back(image);
        }

        return gameImages;
    }


    /**
     * @brief Create a grid object
     */
    void createGrid()
    {
        // Instantiate grid object
        Grid grid(8, 14);
        this->m_grid = grid;
        // Update the background
        this->m_grid.setBackground(bitmap_named(this->m_background));
    }

    /**
     * @brief Create a list of games.
     * 
     */
    void createButtons()
    {
        // Call function to get game images.
        m_gameImages = getGameSprites(m_games);

        for (int i = 0; i < m_gameImages.size(); i++)
        {
            if (i == 0)
            {
                this->m_button = new ButtonNode(new GameScreenButton(Button::GAME, m_gameImages[0]));
                this->m_button->config = m_games[0];
            }
            else
            {
                std::string image = m_gameImages[i];
                this->m_button->addBefore(new ButtonNode(new GameScreenButton(Button::GAME, image)));
                this->m_button->getPrev()->config = m_games[i];
            }
        }
    }

    /** 
     * @brief create a tip to display to the user.
     */
    void createTip()
    {
        bitmap bmpTip = bitmap_named("information");
        bitmap_set_cell_details(bmpTip, 50, 50, 4, 3, 12);
        animation_script infoScript = animation_script_named("info-script");
        animation anim = create_animation(infoScript, "rotate");
        drawing_options opt = option_with_animation(anim);

        std::string tipText[3] = {
            "Use the left and right arrow keys to cycle through the carousel", 
            "Press escape to return to the main menu", 
            "Press enter to start the game"
        };
        this->m_tip = new Tip(tipText[rand()%3], bmpTip, anim, opt, 3000, 25);
    }

    /**
     * @brief draw the game buttons to the window, using the carousel layout
     */
    void updateCarousel()
    {
        // If menu is sliding then clear the grid.
        if (this->m_menuSliding)
        {
            this->m_grid.clearGrid();
        }
        else {
            if (this->m_button && !this->m_inGame)
            {
                this->m_grid.updateCell(this->m_button->getPrev()->button, 2, 0, 1, false);
                this->m_grid.updateCell(this->m_button->button, 2, 5, 1, false);
                this->m_grid.updateCell(this->m_button->getNext()->button, 2, 10, 1, false);
            }
        }
    }

    /**
     * @brief handle carousel input
     */
    void carouselHandler()
    {
        /// Check for input in selector class.
        this->m_button = this->m_selectorGamesMenu.checkKeyInput(this->m_button, m_gameMenu);
        this->_action = this->m_selectorGamesMenu.checkForSelection(this->m_button, m_gameMenu);

#ifdef _WIN32
        check_game_exit();
#endif

        if (this->m_button)
        {
            if (this->_action == "escape" && _overlayActive)
            {
                _overlayActive = false;
            }
            else if (this->_action == "return")
            {
                if (_overlayActive)
                {
#ifdef _WIN32
                    // Get game path
                    _gamePath = (this->button->config.folder() + "/" + this->button->config.exe()).c_str();
                    // Get executable name
                    _gameExe = strdup(this->button->config.exe().c_str());
                    // Get game directory
                    _gameDir = this->button->config.folder().c_str();
#endif

                    // Set the center of the game
                    this->m_x = m_centreX;
                    this->m_y = m_centreY;

                    // fade to black
                    fade(0, 1, 0.1);

                    // fill with black
                    fill_rectangle(rgba_color(0.0, 0.0, 0.0, 1.0), 0, 0, ARCADE_MACHINE_RES_X, ARCADE_MACHINE_RES_Y);
                    // clear grid
                    this->m_grid.clearGrid();
                    // set new background
                    this->m_grid.setBackground(bitmap_named("in_game_bgnd"));
                    //turn off overlay
                    this->_overlayActive = false;
                    // turn off menu music
                    fade_music_out(1000);
                    // fade back in
                    fade(1, 0, 0.1);

#ifdef _WIN32
                    // Call method to open game executable
                    start_game(_gamePath, _gameExe, _gameDir);
#endif

                    return;
                }
                _overlayActive = true;
            }
        }
    }

    /**
     * @brief draw the menu page
     */
    void drawMenuPage()
    {
        // if the game has ended, go back to games menu
        if(!this->m_inGame && this->m_gameStarted)
        {
            this->m_gameStarted = false;
            backToGamesMenu();
        }
        
        this->m_grid.drawGrid();
        
        // Wait for selector to key input to determine slide direction.
        if (m_selectorGamesMenu.getSlideLeft())
            drawUpdateSlideLeft();
        else if (m_selectorGamesMenu.getSlideRight())
            drawUpdateSlideRight();

        if (_overlayActive && !m_menuSliding)
            drawOverlay(m_button->config);
        this->m_tip->draw();

        updateCarousel();
        carouselHandler();
    }

    /**
     * @brief Method to update the sprite positions and draw sprite.
     * 
     * @param buttonSprite The buttons sprite.
     * @param position The position to move the sprite.
     * @return ** void 
     */
    void updateSlide(sprite buttonSprite, int position)
    {
        // Show the base layer of sprite.
        sprite_show_layer(buttonSprite, 0);
        // Set the x position of sprite.
        sprite_set_x(buttonSprite, position);
        // Set the y position of sprite.
        sprite_set_y(buttonSprite, m_posY);
        // draw sprite to screen.
        draw_sprite(buttonSprite);
        // Updatse sprite.
        update_sprite(buttonSprite);
    }

    /**
     * @brief Slide the game buttons on left key input.
     * 
     * @return ** void 
     */
    void drawUpdateSlideLeft()
    {
        this->m_menuSliding = true;

        // Get sprites of buttons on display.
        this->m_newButton1 = this->m_button->getNext()->button->btn();
        this->m_newButton2 = this->m_button->button->btn();
        this->m_newButton3 = this->m_button->getPrev()->button->btn();

        // Increment the x position of sprite.
        this->m_pos1 += m_speed;
        this->m_pos2 += m_speed;
        this->m_pos3 += m_speed;

        // Update and draw sprite.
        updateSlide(this->m_newButton1, this->m_pos1);
        updateSlide(this->m_newButton2, this->m_pos2);
        updateSlide(this->m_newButton3, this->m_pos3);

        // If sprite reaches position.
        if (this->m_pos1 > 1300)
        {
            // Set selector bool back to false.
            m_selectorGamesMenu.setSlideLeft(false);
            this->m_menuSliding = false;
            // Reset positions.
            this->m_pos1 = m_posX;
            this->m_pos2 = this->m_pos1 - m_posX;
            this->m_pos3 = this->m_pos2 - m_posX;
        }
    }

    /**
     * @brief Slide the game buttons on right key input.
     * 
     * @return ** void 
     */
    void drawUpdateSlideRight()
    {
        this->m_menuSliding = true;

        // Get sprites of buttons on display.
        this->m_newButton1 = this->m_button->getPrev()->button->btn();
        this->m_newButton2 = this->m_button->button->btn();
        this->m_newButton3 = this->m_button->getNext()->button->btn();

        // Decrease the x position of sprite.
        this->m_pos1 -= m_speed;
        this->m_pos4 -= m_speed;
        this->m_pos5 -= m_speed;

        // Update and draw sprite.
        updateSlide(this->m_newButton1, this->m_pos1);
        updateSlide(this->m_newButton2, this->m_pos4);
        updateSlide(this->m_newButton3, this->m_pos5);

        if (this->m_pos1 <= 20)
        {
            // Set selector bool back to false.
            m_selectorGamesMenu.setSlideRight(false);
            this->m_menuSliding = false;
            // Reset positions.
            this->m_pos1 = m_posX;
            this->m_pos4 = m_posX * 2;
            this->m_pos5 = m_posX * 3;
        }
    }
    /**
     * @brief Draw an overlay over the game, using data from the config.
     * 
     * @param config the game config.
     */
    void drawOverlay(ConfigData config)
    {
        int xOffset = (current_window_width() / 2) + (current_window_width() / 14);
        int yStart = current_window_height() / 6;
        int yOffset = current_window_height() / 40;

        fill_rectangle(rgba_color(0.0, 0.0, 0.0, 0.8), (current_window_width() / 2), 0, (current_window_width() / 2), current_window_height());
        draw_text(config.title(), COLOR_WHITE, "font_title", yOffset * 3, xOffset, yStart);
        yStart += yOffset * 3;
        draw_text("Author: " + config.author(), COLOR_WHITE, "font_text", yOffset, xOffset, yStart + (1 * yOffset));
        draw_text("Genre: " + config.genre(), COLOR_WHITE, "font_text", yOffset, xOffset, yStart + (2 * yOffset));
        draw_text("Language: " + config.language(), COLOR_WHITE, "font_text", yOffset, xOffset, yStart + (3 * yOffset));
        draw_text("Rating: " + config.rating(), COLOR_WHITE, "font_text", yOffset, xOffset, yStart + (4 * yOffset));
        draw_text("Repository: " + config.repo(), COLOR_WHITE, "font_text", yOffset, xOffset, yStart + (5 * yOffset));
    }

#ifdef _WIN32
    /**
     * @brief  Find the game window and bring it to focus, if it exists
     * 
     * @param windowName the name of the window
     * @param timeout time in ms to search for the window
     * @return true/false if window was found.
     */
    bool FocusWindow(std::string windowName, int timeout = 2000)
    {
        LPCSTR gameWindow =  windowName.c_str();
        HWND gameWindowHandle = NULL;

        int timeElapsed;
        auto startTime = chrono::steady_clock::now();

        //Find the window handle
        do {
            gameWindowHandle = FindWindowEx(NULL,NULL,NULL, gameWindow);
            timeElapsed = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - startTime).count();
            delay(250);
        }
        while (gameWindowHandle == NULL && timeElapsed <= timeout);

        //Maximise the Window
        if (gameWindowHandle != NULL)
        {
            ShowWindow(gameWindowHandle, SW_SHOWMAXIMIZED);
            return true;
        }
        else
        {
            write_line("Unable to find gameWindow Handle");
            return false;
        }
        return true;
    }

    /**
     * @brief Starts up the selected game by starting a new process.
     * 
     * @param gamePath The filepath of the game to open.
     * @param gameExe The executable of the game.
     * @param gameDirectory // The directory of the game.
     * @return ** void 
     */
    void start_game(LPCSTR gamePath,LPSTR gameExe, LPCSTR gameDirectory)
    {
        if (!this->_in_game)
        {
            // Additional info
            STARTUPINFOA startupInfo;

            // Set the size of the structures
            ZeroMemory(&startupInfo, sizeof(startupInfo));
            startupInfo.cb = sizeof(startupInfo);
            ZeroMemory(&processInfo, sizeof(processInfo));

            // Start the program up
            WINBOOL gameProcess = CreateProcessA
            (
                gamePath,               // the path
                gameExe,                // Command line
                NULL,                   // Process handle not inheritable
                NULL,                   // Thread handle not inheritable
                FALSE,                  // Set handle inheritance to FALSE
                NORMAL_PRIORITY_CLASS,     // Don't open file in a separate console
                NULL,                    // Use parent's environment block
                gameDirectory,           // Use parent's starting directory
                &startupInfo,            // Pointer to STARTUPINFO structure
                &processInfo           // Pointer to PROCESS_INFORMATION structure
            );

            OpenProcess(PROCESS_QUERY_INFORMATION,TRUE, gameProcess);

            std::string windowName = gameExe;
            //Remove the extension from the application name (.exe)
            windowName = windowName.substr(0, windowName.find("."));
            //Focus the window
            FocusWindow(windowName);

            this->_in_game = true;
        }
    }

   /**
     * @brief Waits for game to exit.
     * 
     * @return ** void 
     */
    void check_game_exit()
    {
        if (this->_in_game == true)
        {
            this->_game_started = true;
            // Check if game has been exited.
            this->_program_exit = GetExitCodeProcess(processInfo.hProcess, &exit_code);
            if ((this->_program_exit) && (STILL_ACTIVE != exit_code))
            {
                this->_in_game = false;
            }
        }
    }
#endif

    /** 
     * @brief Fade back to games menu
     */
    void backToGamesMenu()
    {
        // fade to black
        fade(0, 1, 0.1);
        fill_rectangle(rgba_color(0.0, 0.0, 0.0, 1.0), 0, 0, ARCADE_MACHINE_RES_X, ARCADE_MACHINE_RES_Y);
        this->m_grid.setBackground(bitmap_named("games_dashboard"));
        // fade to normal
        fade(1, 0, 0.1);
    }

    /**
     * @brief Creates a fading effect
     * 
     * @param alphaStart The starting alpha value.
     * @param alphaEnd The ending alpha value.
     * @param alphaStep The alpha value to increment/decrement by.
     */
    void fade(double alphaStart, double alphaEnd, double alphaStep)
    {
        if (alphaStart > alphaEnd)
            alphaStep = -abs(alphaStep);
        // Calculate the number of steps required to complete the fade.
        double difference = abs(alphaEnd - alphaStart);
        int steps = difference / abs(alphaStep);

        for (int i = 0; i < steps; i++)
        {
            clear_screen();
            this->m_grid.drawGrid();
            // Alpha value manipulates to the opacity of the rectangle.
            fill_rectangle(rgba_color(0.0, 0.0, 0.0, alphaStart), 0, 0, ARCADE_MACHINE_RES_X, ARCADE_MACHINE_RES_Y);
            // Update the alpha value.
            alphaStart += alphaStep;
            refresh_screen(60);

            delay(50);
        }
    }
};

#endif