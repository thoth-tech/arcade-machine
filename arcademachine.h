// Arcade Machine Class
using namespace std;
using std::vector;

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
        vector<ConfigData> _configs;
        /// Vector of MenuButtons
        vector<Button*> _menu_btns;
        /// Vector of GameScreenButtons
        vector<Button*> _game_btns;
        /// Thoth Tech Company intro
        Splashscreen _intro_thoth_tech;
        /// Arcade Machine Team intro
        Splashscreen _intro_arcade_team;
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
        string _action;
        /// Play clicked
        bool _play_clicked = false;
        /// Turn menu music on/off
        bool _play_music = true;
        /// Menu button node
        ButtonNode *menu_button_node = nullptr;


    public:
        // Default Constructor
        ArcadeMachine()
        {
            // Instantiate fundamental objects
            Helper helper;
            ConfigData config;
            Selector cursor("cursor");
            Splashscreen intro_thoth_tech("intro_thoth_tech");
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
        auto get_intro_thoth_tech() const -> const Splashscreen& { return this->_intro_thoth_tech; }
        auto get_intro_arcade_team() const -> const Splashscreen& { return this->_intro_arcade_team; }

        /**
            Starts the Main Menu
        */
        void main_menu()
        {
            while (1)
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

                // Check for left or right input and call slide function.
                if (menu.get_slide_left() == true)
                    menu.draw_update_slide_left();

                if (menu.get_slide_right() == true)
                    menu.draw_update_slide_right();

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
            cell play = this->_grid.GetCell(2, 10);
            cell options = this->_grid.GetCell(3, 10);
            cell exit = this->_grid.GetCell(4, 10);
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
            const string image = path_to_resource("thoth", IMAGE_RESOURCE);
            // Load menu background
            bitmap thoth = load_bitmap("thoth", image);
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

        /** 
            Draws a Splashscreen, plays a sound and
            incremently fills the screen white to animate fading

            @param screen The Splashscreen to draw to screen
            @param sound1 (optional) The first sound to play
            @param sound2 (optional) The second sound to play 
        */
        void intro_animation(Splashscreen screen, string sound1 = "", string sound2 = "")
        {
            // Set fade increment (opacity)
            double alpha = 1.0;
            // Set iterations
            int i = 60;
            // Play Thoth Tech Company sound 
            play_sound_effect(sound1);

            while(i != 0)
            {
                process_events();
                clear_screen();
                // Draw logo
                screen.draw_title_page();
                // Fill screen with white at alpha value (opacity)
                fill_rectangle(rgba_color(1.0, 1.0, 1.0, alpha), 0, 0, 1920, 1080);
                // Decrement i and alpha 
                i--; alpha = alpha - 0.05;
                // If alpha is == 0, hold image for 1.5 seconds
                if (abs(alpha - 0.0) < 1e-9)
                {
                    play_sound_effect(sound2);
                    Sleep(2000);
                    /*  After this has happened, the alpha value will continue into the negatives
                        The colour function continues to accept negative alpha values, 
                        effectively creating a fade out animation for the remainder of the while loop
                    */
                }
                refresh_screen(60);
                Sleep(50);
            }
        }

        /**
            Draws the Splashkit Productions logo to the screen and 
            fetches new games from Git repo
        */
        void intro_splashkit()
        {
            // Pull the most recent version of the arcade-games repo.
            do
            {
                // Draw SplashKit productions screen
                this->_intro_splashkit.draw_title_page();
                draw_text("Loading...", COLOR_SLATE_GRAY, "font_text", 60, WIDTH/2 - 100, HEIGHT/2 + 350);
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
