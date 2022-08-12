#ifndef ARCADE_MACHINE_H
#define ARCADE_MACHINE_H

#ifdef _WIN32
#define ARCADE_MACHINE_PATH_SEP "\\"
#else
#define ARCADE_MACHINE_PATH_SEP "/"
#endif

#define ARCADE_MACHINE_SCALING_FACTOR 1
#define ARCADE_MACHINE_RES_X 1920 * ARCADE_MACHINE_SCALING_FACTOR
#define ARCADE_MACHINE_RES_Y 1080 * ARCADE_MACHINE_SCALING_FACTOR

// Arcade Machine Class
#include "Button.h"
#include "ButtonList.h"
#include "ConfigData.h"
#include "GridLayout.h"
#include "Helper.h"
#include "Menu.h"
#include "Option.h"
#include "Splashscreen.h"
#include "Selector.h"
#include "splashkit.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

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
    Helper _helper;

    /// Instance of ConfigData
    ConfigData _config;

    /// Vector of ConfigData
    std::vector<ConfigData> _configs;

    /// Vector of MenuButtons
    std::vector<Button*> _menu_btns;

    /// Vector of GameScreenButtons
    std::vector<Button*> _game_btns;

    /// Thoth Tech Company intro
    Splashscreen _intro_thoth_tech;

    /// Arcade Machine Team intro
    Splashscreen _intro_arcade_team;

    /// Arcade Machine Team developer names
    std::vector<std::string> m_arcadeTeamDeveloperNames;

    /// SplashKit Production intro
    Splashscreen _intro_splashkit;

    /// Main Menu Selector
    Selector _selector_main_menu;

    /// Games Menu Selector
    Selector _selector_games_menu;

    /// Instance of Grid
    Grid _grid;

    /// Mouse pointer
    point_2d _mouse;

    /// Button Action 
    std::string _action;

    /// Play clicked
    bool _play_clicked = false;

    /// Turn menu music on/off
    bool _play_music = true;

    /// Menu button node
    ButtonNode *menu_button_node = nullptr;

    // Helper function to load developer names into m_arcadeTeamDeveloperNames
    // Called in the constructor
    void loadDeveloperNames(const char* file_path)
    {
        std::string line;
        std::ifstream developerNamesFile(file_path);
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

public:
    // Default Constructor
    ArcadeMachine()
    {
        // Instantiate fundamental objects
        Helper helper;
        ConfigData config;
        Selector cursor("cursor");
        Splashscreen intro_thoth_tech("intro_thoth_tech");

        // load developer names into m_arcadeTeamDeveloperNames
        loadDeveloperNames("developer_names.txt");
        
        Splashscreen intro_arcade_machine_team("intro_arcade_team");
        Splashscreen intro_splashkit("intro_splashkit");
        // Set objects to private properties
        this->_helper = helper;
        this->_config = config;
        this->_selector_main_menu = cursor;
        this->_intro_thoth_tech = intro_thoth_tech;
        this->_intro_arcade_team = intro_arcade_machine_team;
        this->_intro_splashkit = intro_splashkit;
    }

    // Destructor
    ~ArcadeMachine(){}

    // Getters
    auto get_configs() const -> const vector<ConfigData>& { return this->_configs; }

    /**
        Starts the Main Menu
    */
    void main_menu()
    {
        while (! quit_requested())
        {
            process_events();
            clear_screen();
            draw_main_menu();
            button_clicked(this->_mouse);
            refresh_screen(60);
        }
    }

    /**
        Starts the Games Menu
    */
    void games_menu()
    {
        // Instantiate new menu
        Menu menu(this->_configs);
        bool overlayActive = menu.get_overlay_state();
        write_line("got configs");
        menu.create_grid();
        menu.create_buttons();
        menu.create_tip();
        write_line("got buttons");
        write_line("set image");
        this->_game_btns = menu.get_buttons();
        
        while ((!key_typed(ESCAPE_KEY) && !overlayActive) || overlayActive)
        {
            //write_line("into while");
            overlayActive = menu.get_overlay_state();
            process_events();   
            clear_screen();
            // Get mouse position
            this->_mouse = mouse_position();
            // Draw games menu
            menu.draw_menu_page();
            // Check input
            //  this->_action = this->_selector_games_menu.check_key_input(this->_games_btns);
            refresh_screen(60);
        }
    }

    /**
        Starts the Options Menu
    */
    void options_menu()
    {
        Option options;
        Audio *audio = new Audio();
        bool has_background_music = false;
        
        while (!key_down(ESCAPE_KEY))
        {
            process_events();
            clear_screen();

            options.updateOption();

            if(!has_background_music)
            {
                audio->playMusic(options.getCurrentMusic(), options.getVolume());
                has_background_music=false;   
            }
            
            if(options.isChangeMusic())
            {
                has_background_music=false;
            }

            if(options.isChangeVoLume())
            {
                audio->setVolume(options.getVolume());
            }
            refresh_screen(60);
        }
        fade_music_out(500);
    }
    
    /**
        Checks for buttons clicked

        @param point The mouse pointer location on screen
    */
    void button_clicked(point_2d point)
    {
        // Play
        if ( this->_action == "play" || (sprite_at(this->_menu_btns[0]->btn(), point) && mouse_clicked(LEFT_BUTTON)) )
        {
            games_menu();
            write_line("Play button clicked");
        }

        // Options
        else if ( this->_action == "options" || (sprite_at(this->_menu_btns[1]->btn(), point) && mouse_clicked(LEFT_BUTTON)) )
        {
            options_menu();
            write_line("Options button clicked");
        }

        // Exit
        else if ( this->_action == "exit" || (sprite_at(this->_menu_btns[2]->btn(), point) && mouse_clicked(LEFT_BUTTON)) )
        {
            write_line("Exit button clicked");
            exit_program();
        }
    }

    /**
        Draws the Main Menu
    */
    void draw_main_menu() 
    {
        // Get mouse position
        this->_mouse = mouse_position();
        this->_grid.DrawGrid();
        // Draw cursor
        draw_sprite(this->_selector_main_menu.get_cursor());
        // Get button postions
        Cell play = this->_grid.GetCell(2, 10);
        Cell options = this->_grid.GetCell(3, 10);
        Cell exit = this->_grid.GetCell(4, 10);
        // Arcade Machine title
        draw_text("Arcade",  COLOR_BLACK, "font_title", 100, 1180, 100);
        draw_text("Machine", COLOR_BLACK, "font_title", 100, 1150, 200);
        // Draw text on buttons
        draw_text("play!", COLOR_BLACK, "font_btn", 70, play.button->x() + (play.button->centre_x()/2) + 5, play.button->y() + 5);
        draw_text("options", COLOR_BLACK, "font_btn", 70, options.button->x() + (options.button->centre_x()/2) - 20, options.button->y() + 5);
        draw_text("exit", COLOR_BLACK, "font_btn", 70, exit.button->x() + (exit.button->centre_x()/2) + 20, exit.button->y() + 5);

        // Check input in selector class.
        this->menu_button_node = this->_selector_main_menu.check_key_input(this->menu_button_node);

        // Check input in selector class.
        this->_action = this->_selector_main_menu.check_for_selection(this->menu_button_node);
    }

    /**
        Prepares the Main Menu
    */
    void prepare_main_menu()
    {
        // Get the data from the config files.
        this->_configs = this->_helper.config_data_list();
        // Initialise grid 
        Grid grid(ROWS, COLS);
        this->_grid = grid;
        // Create menu buttons
        Button *play = new MenuButton(Button::PLAY, 1.5);
        Button *opts = new MenuButton(Button::OPTS, 1.5);
        Button *exit = new MenuButton(Button::EXIT, 1.5);

        // Add menu buttons to local vector
        this->_menu_btns.push_back(play);
        this->_menu_btns.push_back(opts);
        this->_menu_btns.push_back(exit);

        // Fetch menu background
        bitmap thoth = bitmap_named("thoth");
        // Update grid cells with assets
        this->_grid.SetBackground(thoth);

        // Make new nodes with buttons.
        this->menu_button_node = new ButtonNode(play);
        this->menu_button_node->addAfter(new ButtonNode(opts));
        this->menu_button_node->addBefore(new ButtonNode(exit));

        // Update grid with nodes.
        this->_grid.UpdateCell(menu_button_node->button, 2, 10);
        this->_grid.UpdateCell(menu_button_node->getPrev()->button, 4, 10);
        this->_grid.UpdateCell(menu_button_node->getNext()->button, 3, 10);

        // Play main menu music
        if (this->_play_music) play_music("music_mainmenu");
    }

    /// Plays the Thoth Tech splashscreen animation
    void playThothTechIntro()
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
            _intro_thoth_tech.draw_title_page();
            // Fill screen with white at alpha value (opacity)
            fill_rectangle(rgba_color(1.0, 1.0, 1.0, alpha), 0, 0, ARCADE_MACHINE_RES_X, ARCADE_MACHINE_RES_Y);
            // Decrement i and alpha 
            i--; alpha = alpha - 0.05;
            // If alpha is == 0, hold image for 1.5 seconds
            if (abs(alpha - 0.0) < 1e-9)
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
    void playArcadeTeamIntro()
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
            _intro_arcade_team.draw_title_page();

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
            if (abs(alpha - 0.0) < 1e-9)
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
    void playSplashKitIntro()
    {
        // Pull the most recent version of the arcade-games repo.
        do
        {
            // Draw SplashKit productions screen
            this->_intro_splashkit.draw_title_page();
            draw_text("Loading...", COLOR_SLATE_GRAY, "font_text", 60, ARCADE_MACHINE_RES_X / 2 - 100, ARCADE_MACHINE_RES_Y / 2 + 350);
            refresh_screen();
            
        } while (!this->_config.get_from_git("https://github.com/thoth-tech/arcade-games.git", "games"));
    }

    /**
        Print config data to console
    */
    void print_configs()
    {
        this->_config.print_config_data();
    }

    /**
        Abort this application
    */
    void exit_program()
    {
        exit(EXIT_SUCCESS);
    }
};

#endif