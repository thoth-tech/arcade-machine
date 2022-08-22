#ifndef ARCADE_MACHINE_HELPER_H
#define ARCADE_MACHINE_HELPER_H

#if __cplusplus >= 201703L
#include <filesystem>
namespace fs = std::filesystem;
#else
#define _LIBCPP_NO_EXPERIMENTAL_DEPRECATION_WARNING_FILESYSTEM
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

#include "GridLayout.h"

// Remove definition to use JSON config.
#define ARCADE_MACHINE_USE_TEXT_CONFIG

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
        string getFolderName(string entryPath)
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
        vector<string> getConfigFiles(string dir)
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
        vector<ConfigData> ConfigDataList()
        {   
            vector<string> files = getConfigFiles("./games/games");

            vector<ConfigData> configs;

            for (int i = 0; i < files.size(); i++)
            {
                #ifdef ARCADE_MACHINE_USE_TEXT_CONFIG 
                    ConfigData config(files[i]);
                    string dir = getFolderName(files[i]);
                    config.setFolder(dir);
                    config.setId(i);
                    config.printConfigData();
                    configs.push_back(config);
                #else
                    ConfigData config;
                    string filename = fs::path(files[i]).string();
                    write_line(filename);
                    config.collectJsonData(config.readJson(filename));
                    config.setId(i);
                    config.printConfigData();
                    configs.push_back(config);
                #endif
            }

            return configs;
        }
        
        /**
         * @brief To be used in conjuntion with GridLayoutExample
         * 
         * @param grid 
         */
        void resetScreen(GridLayout grid)
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
        void gridLayoutExample()
        {
            bitmap testBitmap = load_bitmap("test", "appContainer.png");
            open_window("Grid Layout Example", 600, 800);

            //int colsArray[5] = {4, 2, 3, 2, 2};
            int rows = 5;
            int cols = 5;
            //#rows, #cols, ScaletoFit
            GridLayout grid(rows, cols, true);
            //Grid grid(rows,colsArray, true);

            resetScreen(grid);

            int span = cols;
            for (size_t i = 0; i < rows; i++)
            {
                grid.updateCell(testBitmap, i, 0, span);
                --span;
            }
            resetScreen(grid);

            span = 1;
            for (size_t i = 0; i < rows; i++)
            {
                grid.updateCell(testBitmap, i, 0, span);
                ++span;
            }

            resetScreen(grid);

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

            resetScreen(grid);

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

            resetScreen(grid);

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

            resetScreen(grid);

            grid.updateAllCells(testBitmap);
            resetScreen(grid);

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