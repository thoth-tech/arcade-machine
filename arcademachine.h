// Arcade Machine Class

/* 
    This class handles the creation of the
    Arcade Machine itself
*/

using namespace std;
using std::vector;

// Define number of rows and columns in grid
#define ROWS 7
#define COLS 15

class ArcadeMachine
{
    private:
        // Instance of Helper
        Helper _helper;
        // Instance of ConfigData
        ConfigData _config;
        // Vector of ConfigData
        vector<ConfigData> _configs;
        // Thoth Tech Company intro
        Splashscreen _intro_thothtech;
        // SplashKit Production intro
        Splashscreen _intro_splashkit;
        // Instance of Selector
        Selector selector;
        // Instance of Grid
        Grid _grid; 
        // Mouse pointer
        point_2d _mouse;
        // 
    public:
        // Default Constructor
        ArcadeMachine()
        {
            // Instantiate fundamental objects
            Helper helper;
            ConfigData config;
            Splashscreen intro_thothtech("intro_thothtech");
            Splashscreen intro_splashkit("intro_splashkit");
            // Set objects to private properties
            this->_helper = helper;
            this->_config = config;
            this->_intro_thothtech = intro_thothtech;
            this->_intro_splashkit = intro_splashkit;
            // Get the data from the config files.
            this->_configs = helper.config_data_list();
        }
        // Destructor
        ~ArcadeMachine(){}
        // First Overloaded Constructor
        ArcadeMachine(Grid grid){}

        // Getters
        auto get_configs() const -> const vector<ConfigData>& { return this->_configs; }

        /*
            Draw Main Menu
        */
        void draw_main_menu()
        {
            // Get mouse position
            this->_mouse = mouse_position();
            this->_grid.DrawGrid();
            // Draw cursor
            draw_sprite(selector.get_cursor());
            // Get button postions
            cell play = this->_grid.GetCell(2, 10);
            cell options = this->_grid.GetCell(3, 10);
            cell exit = this->_grid.GetCell(4, 10);
            // Draw text on buttons
            draw_text("play!", COLOR_BLACK, "btn_font", 70, play.button->x() + (play.button->centre_x()/2) + 5, play.button->y() + 5);
            draw_text("options", COLOR_BLACK, "btn_font", 70, options.button->x() + (options.button->centre_x()/2) - 20, options.button->y() + 5);
            draw_text("exit", COLOR_BLACK, "btn_font", 70, exit.button->x() + (exit.button->centre_x()/2) + 20, exit.button->y() + 5);
            // Check input
            //this->action = select.check_key_input(this->btns);
            // Draw creators
            draw_text("Created By", COLOR_BLACK, "roboto", 14, 1200, 850);
            draw_text("Sarah", COLOR_BLACK, "roboto", 14, 1200, 870);
            draw_text("Anthony", COLOR_BLACK, "roboto", 14, 1200, 890);
            draw_text("Riley", COLOR_BLACK,  "roboto", 14, 1200, 910);
            draw_text("Huy", COLOR_BLACK, "roboto", 14, 1200, 930);
        }

        /*
            Prepare Main Menu
        */
        void prepare_main_menu()
        {
            // Initialise grid 
            Grid grid(ROWS, COLS);
            this->_grid = grid;
            // Initialise menu
            Menu menu(this->get_configs());
            // Create menu buttons
            Button *play = new MenuButton(Button::PLAY, 1.2);
            Button *options = new MenuButton(Button::OPTIONS, 1.2);
            Button *exit = new MenuButton(Button::EXIT, 1.2);
            // Set menu background
            string image = path_to_resource("thoth", IMAGE_RESOURCE);
            bitmap thoth = load_bitmap("thoth", image);
            // Update grid cells with assets
            grid.UpdateCell(thoth, 0, 0, 1, false);
            grid.UpdateCell(play, 2, 10);
            grid.UpdateCell(options, 3, 10);
            grid.UpdateCell(exit, 4, 10);
            // Play main menu music
            play_music("menu_music");
        }

        /*
            Print config data to console
        */
        void print_configs()
        {
            this->_config.print_config_data();
        }

        /* 
            Draws the Thoth Tech company logo to screen and
            incremently fills the screen white to animate fading
        */
        void play_intro()
        {
            // Create new splashscreen with Thoth Tech logo
            
            // Set fade increment (opacity)
            double alpha = 1.0;
            // Set iterations
            int i = 50;
            // Play sound effect 
            play_sound_effect("intro");
            
            // Do this until iterations finish
            while(i != 0)
            {
                // Draw Thoth Tech company logo
                this->_intro_thothtech.draw_title_page();
                // Fill screen with white at alpha value (opacity)
                fill_rectangle(rgba_color(1.0, 1.0, 1.0, alpha), 0, 0, 1920, 1080);
                // Decrement i and alpha 
                i--; alpha = alpha - 0.05;
                // If alpha is == 0, hold image for 1.5 seconds
                if (abs(alpha - 0.0) < 1e-9)
                    Sleep(1500);
                    /* After this has happened, the alpha value will continue into the negatives
                        The colour function continues to accept negative alpha values, 
                        effectively creating a fade out animation for the remainder of the while loop
                    */
                refresh_screen();
                Sleep(50);
            }
        }

        /*

        */
        void load_games()
        {
            // Pull the most recent version of the arcade-games repo.
            do
            {
                // Draw SplashKit productions screen
                this->_intro_splashkit.draw_title_page();
                draw_text("Loading...", COLOR_SLATE_GRAY, "btn_font", 60, WIDTH/2 - 100, HEIGHT/2 + 350);
                refresh_screen();
                
            } while (!this->_config.get_from_git("https://github.com/thoth-tech/arcade-games.git", "games"));

        }
};
