#ifndef ARCADE_MACHINE_HELPER_H
#define ARCADE_MACHINE_HELPER_H

#include <experimental/filesystem>
#include <cstring>

namespace fs = std::experimental::filesystem;

// To test txt configs, change to "config"
#define CONFIG_DIR "config"

/**
 * @brief Helper class
 * This class has been added to house orphan functions 
 * 
 */
class Helper {
    public:

        /**
         * @brief Find the game folder name to store in config class.
         * 
         * @param entryPath 
         * @return * string 
         */
        string get_folder_name(string entryPath)
        {
            string dir  = fs::path(entryPath).remove_filename().generic_string();
            std::cout << "Game-Directory Path: " << dir << "\n";
            return dir;
        }

        /**
         * @brief Find the config files in the games directory.
         * 
         * @param dir The games directory
         * @return * vector<string> 
         */
        vector<string> get_config_files(string dir)
        {
            vector<string> files;

            for (const auto & entry : fs::recursive_directory_iterator(dir))
            {
                if (entry.path().filename() == "config.txt")
                {
                    std::cout << "Game-Config Path: " << entry.path().generic_string() << "\n";
                    files.push_back(entry.path().generic_string());
                }
            }

            return files;
        }

        /**
         * @brief Create configs vector from config files
         * 
         * @return * vector<ConfigData> 
         */
        vector<ConfigData> config_data_list()
        {   
            vector<string> files = get_config_files("./games/games");

            vector<ConfigData> configs;

            for (int i = 0; i < files.size(); i++)
            {
                if (strcmp(CONFIG_DIR, "config") == 0)
                {
                    ConfigData config(files[i]);
                    string dir = get_folder_name(files[i]);
                    config.setFolder(dir);
                    config.setId(i);
                    config.printConfigData();
                    configs.push_back(config);
                }
                else if (strcmp(CONFIG_DIR, "json") == 0)
                {
                    ConfigData config;
                    string filename = fs::path(files[i]).string();
                    write_line(filename);
                    config.collectJsonData(config.readJson(filename));
                    config.setId(i);
                    config.printConfigData();
                    configs.push_back(config);
                }
            }

            return configs;
        }
        
        /**
         * @brief To be used in conjuntion with GridLayoutExample
         * 
         * @param grid 
         */
        void ResetScreen(Grid grid)
        {
            process_events();
            clear_screen(COLOR_DARK_SLATE_GRAY);
            grid.drawGrid();
            grid.clearGrid();
            refresh_screen();
            delay(1000);
        }
        
        /**
         * @brief To test the grid layout 
         * 
         */
        void GridLayoutExample()
        {
            bitmap testBitmap = load_bitmap("test", "appContainer.png");
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
                grid.updateCell(testBitmap, i, 0, span);
                --span;
            }
            ResetScreen(grid);

            span = 1;
            for (size_t i = 0; i < rows; i++)
            {
                grid.updateCell(testBitmap, i, 0, span);
                ++span;
            }

            ResetScreen(grid);

            bool alternate = true;
            for (size_t i = 0; i < rows; i++)
            {
                for (size_t j = 0; j < cols; j++)
                {
                    if (alternate)
                        grid.updateCell(testBitmap, i, j, 1);
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
                        grid.updateCell(testBitmap, i, j, 1);
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
                        grid.updateCell(testBitmap, i, j, 1);
                    alternate = !alternate;
                }
                alternate = !alternate;
            }

            ResetScreen(grid);

            grid.updateAllCells(testBitmap);
            ResetScreen(grid);

            while (!quit_requested())
            {
                grid.drawGrid();
                process_events();
                clear_screen();
                refresh_screen(60);
            }
        }
};

#endif