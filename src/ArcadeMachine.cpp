#include "ArcadeMachine.h"
#include "Configuration.h"

#include <iostream>
#include <cstdlib>
#include <cmath>

// Helper function to load developer names into m_arcadeTeamDeveloperNames
// Called in the constructor
void ArcadeMachine::loadDeveloperNames(const char* filePath)
{
    std::string line;
    std::ifstream developerNamesFile(filePath);
    if (developerNamesFile.fail())
    {
        std::cout << "error: unable to open developer names file\n";
    }
    else if (developerNamesFile.is_open())
    {
        while(std::getline(developerNamesFile, line))
        {
            m_arcadeTeamDeveloperNames.push_back(line);
        }
    }

    developerNamesFile.close();
}

// Default Constructor
ArcadeMachine::ArcadeMachine()
{
    // Instantiate fundamental objects
    Helper helper;
    ConfigData config;
    Selector cursor("cursor");
    Splashscreen introThothTech("intro_thoth_tech");

    // load developer names into m_arcadeTeamDeveloperNames
    loadDeveloperNames("developer_names.txt");
    
    Splashscreen introArcadeMachineTeam("intro_arcade_team");
    Splashscreen introSplashkit("intro_splashkit");
    Audio audio;
    
    // Set objects to private properties
    this->m_helper = helper;
    this->m_config = config;
    this->m_selectorMainMenu = cursor;
    this->m_introThothTech = introThothTech;
    this->m_introArcadeTeam = introArcadeMachineTeam;
    this->m_introSplashkit = introSplashkit;
    this->audio = audio;
}

ArcadeMachine::~ArcadeMachine()
{
    std::cout << "Destructor called on ArcadeMachine\n";
    std::cout << "ArcadeMachine: clearing memory...\n";
    for (const auto& button : m_menuBtns) delete button;
    m_menuBtns.clear();
    for (const auto& button : m_gameBtns) delete button;
    m_gameBtns.clear();
    m_grid.destroy();
}

/**
    Starts the Main Menu
*/
void ArcadeMachine::mainMenu()
{
    while (!quit_requested())
    {
        this->audio.playMusic(this->audio.getCurrentMusic());
        audio.set_volume_for_machine();
        if(key_typed(P_KEY)){
            write_line(audio.getVolume());
        }
        process_events();
        clear_screen();
        drawMainMenu();
        buttonClicked(this->m_mouse);
        refresh_screen(60);
    }
}

/**
    Starts the Games Menu
*/

void ArcadeMachine::gamesMenu()
{
    // Instantiate new menu
    Menu menu(this->m_configs);
    bool overlayActive = menu.getOverlayState();
    write_line("got configs");
    menu.createGrid();
    menu.createButtons();
    menu.createTip();
    write_line("got buttons");
    write_line("set image");
    this->m_gameBtns = menu.getButtons();
    
    while ((!key_typed(ESCAPE_KEY) && !overlayActive) || overlayActive)
    {
        //write_line("into while");
        overlayActive = menu.getOverlayState();
        process_events();   
        clear_screen();
        // Get mouse position
        this->m_mouse = mouse_position();
        // Draw games menu
        menu.drawMenuPage();
        // Check input
        //  this->_action = this->m_selectorGamesMenu.check_key_input(this->_games_btns);
        refresh_screen(60);
    }
}
/**
    Starts the Options Menu
*/
void ArcadeMachine::optionsMenu()
{
    Option options;
    bool has_background_music = false;
    options.createOptionsButtons();
    
    while (this->m_exitOptions == false)
    {
        process_events();
        clear_screen();
        //options.updateOption();
        options.drawOptionsMenu();
        this->m_exitOptions = options.checkAction(this->audio);  
        
        if(options.isChangeMusic())
        {
            has_background_music=false;
        }

        if(options.isChangeVoLume())
        {
            //audio->setVolume(options.getVolume());
        }

        refresh_screen(60);
    }

    this->m_exitOptions = false;

    //fade_music_out(500);
}

/**
    Checks for buttons clicked

    @param point The mouse pointer location on screen
*/
void ArcadeMachine::buttonClicked(point_2d point)
{
    // Play
    if ( this->m_action == "play" || (sprite_at(this->m_menuBtns[0]->btn(), point) && mouse_clicked(LEFT_BUTTON)) )
    {
        gamesMenu();
        write_line("Play button clicked");
    }

    // Options
    else if ( this->m_action == "options" || (sprite_at(this->m_menuBtns[1]->btn(), point) && mouse_clicked(LEFT_BUTTON)) )
    {
        optionsMenu();
        write_line("Options button clicked");
    }

    // Exit
    else if ( this->m_action == "exit" || (sprite_at(this->m_menuBtns[2]->btn(), point) && mouse_clicked(LEFT_BUTTON)) )
    {
        write_line("Exit button clicked");
        exitProgram();
    }
}

/**
    Draws the Main Menu
*/
void ArcadeMachine::drawMainMenu() 
{
    // Get mouse position
    this->m_mouse = mouse_position();
    this->m_grid.drawGrid();
    // Draw cursor
    this->m_selectorMainMenu.setRenderCursor(true);
    draw_sprite(this->m_selectorMainMenu.getCursor());
    // Get button postions
    Cell play = this->m_grid.getCell(2, 10);
    Cell options = this->m_grid.getCell(3, 10);
    Cell exit = this->m_grid.getCell(4, 10);
    // Arcade Machine title
    draw_text("Arcade",  COLOR_BLACK, "font_title", 100, 1180, 100);
    draw_text("Machine", COLOR_BLACK, "font_title", 100, 1150, 200);
    // Draw text on buttons
    draw_text("play!", COLOR_BLACK, "font_btn", 70, play.button->x() + (play.button->centreX()/2) + 5, play.button->y() + 5);
    draw_text("options", COLOR_BLACK, "font_btn", 70, options.button->x() + (options.button->centreX()/2) - 20, options.button->y() + 5);
    draw_text("exit", COLOR_BLACK, "font_btn", 70, exit.button->x() + (exit.button->centreX()/2) + 20, exit.button->y() + 5);

    // Check input in selector class.
    this->m_menuButtonNode = this->m_selectorMainMenu.checkKeyInput(this->m_menuButtonNode);

    // Check input in selector class.
    this->m_action = this->m_selectorMainMenu.checkForSelection(this->m_menuButtonNode);
}

/**
    Prepares the Main Menu
*/
void ArcadeMachine::prepareMainMenu()
{
    // Get the data from the config files.
    this->m_configs = this->m_helper.ConfigDataList();
    
    // Initialise grid 
    GridLayout grid(ROWS, COLS);
    this->m_grid = grid;

    // Create menu buttons
    Button *play = new MenuButton(Button::PLAY, 1.5);
    Button *opts = new MenuButton(Button::OPTS, 1.5);
    Button *exit = new MenuButton(Button::EXIT, 1.5);

    // Add menu buttons to local vector
    this->m_menuBtns.push_back(play);
    this->m_menuBtns.push_back(opts);
    this->m_menuBtns.push_back(exit);

    // Fetch menu background
    bitmap thoth = bitmap_named("thoth");
    
    // Update grid cells with assets
    this->m_grid.setBackground(thoth);

    // Make new nodes with buttons.
    this->m_menuButtonNode = new ButtonNode(play);
    this->m_menuButtonNode->addAfter(new ButtonNode(opts));
    this->m_menuButtonNode->addBefore(new ButtonNode(exit));

    // Update grid with nodes.
    this->m_grid.updateCell(m_menuButtonNode->button, 2, 10);
    this->m_grid.updateCell(m_menuButtonNode->getPrev()->button, 4, 10);
    this->m_grid.updateCell(m_menuButtonNode->getNext()->button, 3, 10);

    // Play main menu music
    // if (this->m_playMusic) play_music("music_mainmenu");
    
}

/// Plays the Thoth Tech splashscreen animation
void ArcadeMachine::playThothTechIntro()
{
    // Set fade increment (opacity)
    double alpha = 1.0;
    // Set iterations
    int i = 60;
    // Play Thoth Tech Company sound 
    play_sound_effect("intro_thoth");

    while(i != 0)
    {
        process_events();
        clear_screen();
        // Draw logo
        m_introThothTech.drawTitlePage();
        // Fill screen with white at alpha value (opacity)
        fill_rectangle(rgba_color(1.0, 1.0, 1.0, alpha), 0, 0, ARCADE_MACHINE_RES_X, ARCADE_MACHINE_RES_Y);
        // Decrement i and alpha 
        i--; alpha = alpha - 0.05;
        // If alpha is == 0, hold image for 1.5 seconds
        if (std::abs(alpha - 0.0) < 1e-9)
        {
            delay(2000);
            /*  After this has happened, the alpha value will continue into the negatives
                The colour function continues to accept negative alpha values, 
                effectively creating a fade out animation for the remainder of the while loop
            */
        }
        refresh_screen(60);
        delay(50);
    }
}

/// Plays the Arcade Team splashscreen animation
void ArcadeMachine::playArcadeTeamIntro()
{
    // Set fade increment (opacity)
    double alpha = 1.0;
    // Set iterations
    int i = 60;
    // Play Thoth Tech Company sound 
    play_sound_effect("intro_coin");

    while(i != 0)
    {
        process_events();
        clear_screen();

        // Draw logo
        m_introArcadeTeam.drawTitlePage();

        int developerNameSpacing = 32;
        int developerNameIndex = 0;
        for  (const auto& developerName : m_arcadeTeamDeveloperNames)
        {
            draw_text(developerName,
                COLOR_BLACK,
                "font_text",
                26,
                (ARCADE_MACHINE_RES_X / 2) - 180,
                (ARCADE_MACHINE_RES_Y / 2) + 220 + developerNameSpacing * developerNameIndex);
            developerNameIndex++;
        }

        // Fill screen with white at alpha value (opacity)
        fill_rectangle(rgba_color(1.0, 1.0, 1.0, alpha), 0, 0, ARCADE_MACHINE_RES_X, ARCADE_MACHINE_RES_Y);
        // Decrement i and alpha 
        i--; alpha = alpha - 0.05;
        // If alpha is == 0, hold image for 1.5 seconds
        if (std::abs(alpha - 0.0) < 1e-9)
        {
            play_sound_effect("intro_start");
            delay(2000);
            /*  After this has happened, the alpha value will continue into the negatives
                The colour function continues to accept negative alpha values, 
                effectively creating a fade out animation for the remainder of the while loop
            */
        }
        refresh_screen(60);
        delay(50);
    }
}

/**
    Draws the Splashkit Productions logo to the screen and 
    fetches new games from Git repo
*/
void ArcadeMachine::playSplashKitIntro()
{
    // Pull the most recent version of the arcade-games repo.
    do
    {
        // Draw SplashKit productions screen
        this->m_introSplashkit.drawTitlePage();
        draw_text("Loading...", COLOR_SLATE_GRAY, "font_text", 60, ARCADE_MACHINE_RES_X / 2 - 100, ARCADE_MACHINE_RES_Y / 2 + 350);
        refresh_screen();
        
    } while (!this->m_config.getFromGit("https://github.com/thoth-tech/arcade-games.git", "games"));
}

/**
    Print config data to console
*/
void ArcadeMachine::printConfigs()
{
    this->m_config.printConfigData();
}

/**
    Abort this application
*/
void ArcadeMachine::exitProgram()
{
    exit(EXIT_SUCCESS);
}
