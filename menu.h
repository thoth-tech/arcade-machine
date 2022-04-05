#include "includes.h"
#include "game.h"
#include "cdata.h"

class Menu {
    private:
        sprite background = create_sprite("background.png");
        vector<Game> games;
        vector<sprite> gameImages;
        PROCESS_INFORMATION processInfo;
        DWORD exitCode;
        LPCSTR _gamePath;
        LPSTR _gameExe;
        LPCSTR _gameDir;
        double center_x = 960;
        double center_y = 540;
        double x;
        double y;
        bool inGame = false;
        bool programExit;

    public: 
        // Get the details from the config data and store in games vector.
        void get_game_details(vector<ConfigData> configs)
        {
            for (int i = 0; i < configs.size(); i++)
            {
                Game *game = new Game(configs[i].id(), configs[i].image(), configs[i].title(), 
                                        configs[i].genre(), configs[i].rating(), configs[i].author(), 
                                        configs[i].exe(), configs[i].folder());

                games.push_back(*game);
            }
        }

        // Draw the game image buttons on the window.  
        void set_game_image()
        {
            int x = 100;
            int y = 100;
            for (int i = 0; i < games.size(); i++)
            {
                // Get image dir and image name from games vector and store the id.
                string image = games[i].folder() + "/" + games[i].image();
                sprite gameImageSprite = create_sprite(image);
                gameImages.push_back(gameImageSprite);

                point_2d gamePosition;
                gamePosition.x = x;
                gamePosition.y = y;

                sprite_set_position(gameImageSprite, gamePosition);
                sprite_set_scale(gameImageSprite, 0.3);
                draw_sprite(gameImageSprite, 0, 0);

                x = x + 500;
            }
        }

        // Draw the background and call set game image.
        void draw_menu_page()
        {
            draw_sprite(background, 0, 0);

            set_game_image();
        }

        // Determine which game button object is clicked on and get the game info for start game.
        void button_clicked(point_2d point)
        {
            for (int i = 0; i < gameImages.size(); i++)
            {
                if (sprite_at(gameImages[i], point))
                {
                    // If the mouse is then clicked.
                    if (mouse_clicked(LEFT_BUTTON))
                    {
                        _gamePath = (games[i].folder() + "/" + games[i].exe()).c_str();
                        _gameExe = strdup(games[i].exe().c_str());
                        _gameDir = games[i].folder().c_str();

                        x = center_x;
                        y = center_y;

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

            inGame = true;
        }

        // Method to keep the mouse positioned within the game window.
        void move_mouse_position(point_2d mousePoint)
        {
            if (inGame == true)
            {
                // Check if game has been exited.
                programExit = GetExitCodeProcess(processInfo.hProcess, &exitCode);
                if ((programExit) && (STILL_ACTIVE != exitCode))
                {
                    inGame = false;
                }
                // If game is not exited, keep the mouse pointer in the game window.
                int mouse_x = mousePoint.x;
                int mouse_y = mousePoint.y;
                if (mousePoint.x > x || mousePoint.x < x || mousePoint.y > y || mousePoint.y < y)
                {
                    move_mouse(center_x, center_y);
                    x = mouse_x;
                    y = mouse_y;
                }
            }
        }

};
