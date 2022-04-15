// Helper class
// Any generic functions can go here

namespace fs = std::experimental::filesystem;
using namespace std;
using std::vector;

// To test txt configs, change to "config"
#define CONFIG_DIR "config"

class Helper{
    public:
        char* string_to_char(string str)
        {
            int n = str.length();
            char* name[n+1];
            strcpy(*name, str.c_str());
            return *name;
        }

        // Method to find the game folder name to store in config class.
        string get_folder_name(string entryPath)
        {
            ConfigData config;

            unsigned lastPath = entryPath.find("\\", 8);
            string dir = entryPath.substr(0, lastPath);
            unsigned findBackSlash = dir.find("\\");
            dir.replace(findBackSlash, 1, "/");

            return dir;
        }

        // Method to find the config files in the games directory.
        vector<string> get_config_files(string dir)
        {
            vector<string> files;

            for (const auto & entry : fs::recursive_directory_iterator(dir))
            {
                if (entry.path().filename() == "config.txt")
                {
                    std::cout << entry << '\n';
                    files.push_back(entry.path().string());
                    cout << entry.path().string() << endl;
                    string path = entry.path().string();
                }
            }

            return files;
        }

        // Create configs vector from config files.
        vector<ConfigData> config_data_list()
        {   
            vector<string> files = get_config_files("./games");

            vector<ConfigData> configs;

            for (int i = 0; i < files.size(); i++)
            {
                if (strcmp(CONFIG_DIR, "config") == 0)
                {
                    ConfigData config(files[i]);
                    string dir = get_folder_name(files[i]);
                    config.set_folder(dir);
                    config.set_id(i);
                    config.print_config_data();
                    configs.push_back(config);
                }
                else if (strcmp(CONFIG_DIR, "json") == 0)
                {
                    ConfigData config;
                    string filename = fs::path(files[i]).string();
                    write_line(filename);
                    config.collect_json_data(config.read_json(filename));
                    config.set_id(i);
                    config.print_config_data();
                    configs.push_back(config);
                }
            }

            return configs;
        }
        
        void ResetScreen(Grid grid)
        {
            process_events();
            clear_screen(COLOR_DARK_SLATE_GRAY);
            grid.DrawGrid();
            grid.ClearGrid();
            refresh_screen();
            delay(1000);
        }

        void GridLayoutExample()
        {
            load_bitmap("test", "appContainer.png");
            open_window("Grid Layout Example", 600, 800);

            //int colsArray[5] = {4, 2, 3, 2, 2};
            int rows = 5;
            int cols = 5;
            //#rows, #cols, ScaletoFit
            Grid grid(rows, cols, true);
            //Grid grid(rows,colsArray, true);

            ResetScreen(grid);

            int span = cols;
            for (size_t i = 0; i < rows; i++)
            {
                grid.UpdateCell("test", i, 0, span);
                --span;
            }
            ResetScreen(grid);

            span = 1;
            for (size_t i = 0; i < rows; i++)
            {
                grid.UpdateCell("test", i, 0, span);
                ++span;
            }

            ResetScreen(grid);

            bool alternate = true;
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    if (alternate)
                        grid.UpdateCell("test", i, j, 1);
                    alternate = !alternate;
                }
            }

            ResetScreen(grid);

            alternate = false;
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    if (alternate)
                        grid.UpdateCell("test", i, j, 1);
                }
                alternate = !alternate;
            }

            ResetScreen(grid);

            alternate = false;
            for (size_t i = 0; i < cols; i++)
            {
                for (size_t j = 0; j < rows; j++)
                {
                    if (alternate)
                        grid.UpdateCell("test", i, j, 1);
                    alternate = !alternate;
                }
                alternate = !alternate;
            }

            ResetScreen(grid);

            grid.UpdateAllCells("test");
            ResetScreen(grid);

            while (!quit_requested())
            {
                grid.DrawGrid();
                process_events();
                clear_screen();
                refresh_screen(60);
            }
        }

        void play_intro()
        {
            Splashscreen intro_thothtech("intro_thothtech");
            double alpha = 1.0;
            int i = 50;
            play_sound_effect("intro");
            
            while(i != 0)
            {
                intro_thothtech.draw_title_page();
                fill_rectangle(rgba_color(1.0, 1.0, 1.0, alpha), 0, 0, 1920, 1080);
                i--; alpha = alpha - 0.05;

                if (abs(alpha - 0.0) < 1e-9)
                Sleep(1500);
            
                refresh_screen();
                Sleep(50);
            }
        }
};
