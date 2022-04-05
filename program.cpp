#include "includes.h"

using namespace std;
using std::vector;

// To test txt configs, change to "config"
#define CONFIG_DIR "config"

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

    for(int i = 0; i < files.size(); i++)
    {
        if(strcmp(CONFIG_DIR, "config") == 0)
        {
            ConfigData config(files[i]);
            string dir = get_folder_name(files[i]);
            config.set_folder(dir);
            config.set_id(i);
            config.print_config_data();
            configs.push_back(config);
        }
        else if(strcmp(CONFIG_DIR, "json") == 0)
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

// The title screen the user first comes in contact with.
void splashscreen()
{
    Splashscreen splashScreen;
    point_2d mousePoint;
    bool playClicked;

    while (not quit_requested() && (not key_down(ESCAPE_KEY)))
    {
        process_events();

        clear_screen();

        mousePoint = mouse_position();
        splashScreen.draw_title_page();
        splashScreen.button_clicked(mousePoint);

        playClicked = splashScreen.getPlayClick();

        if (playClicked)
        {
            return;
        }

        refresh_screen(60);
    }
}

int main()
{
    ConfigData configData;
    point_2d mousePoint;

    // Pull the most recent version of the arcade-games repo.
    configData.get_from_git("https://github.com/thoth-tech/arcade-games.git", "games");

    // Get the data from the config files.
    vector<ConfigData> configs = config_data_list();
    configData.print_config_data();

    // Pass the config info to the menu class.
    Menu menu(configs);

    // Open window and toggle border off.
    open_window("arcade-machine", 1920, 1080);
    window_toggle_border("arcade-machine");

    // Call the splashscreen.
    splashscreen();

    // Update loop.
    while (not quit_requested() && (not key_down(ESCAPE_KEY)))
    {
        process_events();

        clear_screen();
        // Constantly update mouse pointer position.
        mousePoint = mouse_position();
        // Draw the menu page.
        menu.draw_menu_page();
        // Listen for button click and get mouse location.
        menu.button_clicked(mousePoint);
        // Keep this running while game is played to keep mouse in the game window.
        menu.move_mouse_position(mousePoint);

        refresh_screen(60);
    }

    Button button;

    open_window("", WIDTH, HEIGHT);

    while( not quit_requested() )
        {
            process_events();

            clear_screen();
            
            Button btn0(Button::GREEN, 8, 4);
            Button btn4(Button::YELLOW, 8, 5);
            Button btn2(Button::LIGHT_BLUE, 8, 6);
            Button btn1(Button::RED, 8, 7);

            btn1.draw_screen_guides();

            refresh_screen(60);
        }

    return 0;
}
