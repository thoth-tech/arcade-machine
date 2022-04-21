// Arcade Machine Class

/* 
    This class handles the creation of the
    Arcade Machine itself
*/

using namespace std;
using std::vector;

class ArcadeMachine
{
    private:
        // Instance of Helper class
        Helper _helper;
        // Instance of Helper class
        ConfigData _config;
        // Vector of ConfigData
        vector<ConfigData> _configs;
        // Thoth Tech Company intro
        Splashscreen _intro_thothtech;
        // SplashKit Production intro
        Splashscreen _intro_splashkit;
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

        auto get_configs() const -> const vector<ConfigData>& { return this->_configs; }


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
