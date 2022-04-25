
using namespace std;
using std::vector;

class Menu {
    private:
        string background = "games_dashboard";
        // Vector to store the config data of each game
        vector<ConfigData> _games;
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
        // Used to find x centre of screen
        double _center_x = 960;
        // Used to fine y centre of screen
        double _center_y = 540;
        // Stores x of mouse position
        double _x;
        // Stores y of mouse position
        double _y;
        // Checks if game is running
        bool _in_game = false;
        // Checks if program has exited
        bool _program_exit;
        // Vector of buttons
        vector<Button*> btns;
        // A selector
        Selector select;
        // The action to take
        string action;
        // Vectore to store game images
        vector<string> game_images;
        // Menu grid
        Grid _grid;

    public: 
        Menu(){}

        Menu(vector<ConfigData> configs) 
        {
            this->_games = configs;
        }
        ~Menu(){}

        // Getters
        auto get_buttons() const -> const vector<Button*> { return this->btns; }

        // This function gets the game images from the config files and returns vector of game images.
        vector<string> get_game_sprites(vector<ConfigData> configs) 
        {
            vector<string> game_images;

            for (int i = 0; i < configs.size(); i++)
            {
                // Get image dir and image name from games vector.
                string image = configs[i].folder() + "/" + configs[i].image();
                game_images.push_back(image);
            }

            return game_images;
        }

        void create_grid()
        {
            // Instantiate grid object
            Grid grid(6, 4);
            this->_grid = grid;
        }

        // This function creates the game buttons from the game images.
        void create_buttons()
        {
            // Call function to get game images.
            game_images = get_game_sprites(_games);

            for (int i = 0; i < game_images.size(); i++)
            {
                // Get each image 
                string image = game_images[i];
                // Create new buttons
                Button *button = new GameScreenButton(Button::GAME, image);
                // Add to buttons array
                this->btns.push_back(button);
            }
        }

        // Draw the game image buttons on the window.  
        void set_game_image()
        {
            if (this->btns.size() > 0)
            {
                write_line("buttons are greater than 0");
                // Update the grid cells from the button array
                //grid.UpdateCell(this->btns[0], 1, 1, 0.5);
                this->_grid.UpdateCell(this->btns[1], 0, 1);
                this->_grid.UpdateCell(this->btns[2], 0, 3);
                write_line("updated cells");
                // Draw grid to screen
                this->_grid.DrawGrid();
                write_line("drew grid");
                // Check for key input
                //this->action = select.check_key_input(this->btns);
                //write_line("checking inputs");
            }
        }

        // Draw the background and call set game image.
        void draw_menu_page()
        {
            // Draw background
            draw_bitmap(this->background, 0, 0);
            set_game_image();
        }

        // Determine which game button object is clicked on and get the game info for start game.
        void button_clicked(point_2d point)
        {
            for (int i = 0; i < game_images.size(); i++)
            {
                if (sprite_at(this->btns[i]->btn(), point))
                {
                    // If the mouse is then clicked.
                    if (mouse_clicked(LEFT_BUTTON))
                    {
                        // Get game path
                        _gamePath = (_games[i].folder() + "/" + _games[i].exe()).c_str();
                        // Get executable name
                        _gameExe = strdup(_games[i].exe().c_str());
                        // Get game directory
                        _gameDir = _games[i].folder().c_str();

                        // Set the center of the game
                        this->_x = _center_x;
                        this->_y = _center_y;

                        // Call method to open game executable
                        start_game(_gamePath, _gameExe, _gameDir);
                        return;
                    }
                }
            }
        }

        // Start up the chosen game using CreateProcessA.
        void start_game(LPCSTR gamePath,LPSTR gameExe, LPCSTR gameDirectory)
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

            this->_in_game = true;
        }

        // Method to keep the mouse positioned within the game window.
        void move_mouse_position(point_2d mousePoint)
        {
            if (this->_in_game == true)
            {
                // Check if game has been exited.
                this->_program_exit = GetExitCodeProcess(processInfo.hProcess, &exit_code);
                if ((this->_program_exit) && (STILL_ACTIVE != exit_code))
                {
                    this->_in_game = false;
                }
                // If game is not exited, keep the mouse pointer in the game window.
                int mouse_x = mousePoint.x;
                int mouse_y = mousePoint.y;
                if (mousePoint.x > this->_x || mousePoint.x < this->_x || mousePoint.y > this->_y || mousePoint.y < this->_y)
                {
                    move_mouse(_center_x, _center_y);
                    this->_x = mouse_x;
                    this->_y = mouse_y;
                }
            }
        }

};
