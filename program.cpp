#include "helper.h"

using namespace std;
using std::vector;

// To test txt configs, change to "config"
#define CONFIG_DIR "json"

vector<string> get_config_files(string dir)
{
    vector<string> files;
    for(const auto & entry : fs::directory_iterator(dir))
    {
        files.push_back(entry.path().string());
        cout << entry.path().string() << endl;
    }
    return files;
}

vector<ConfigData> config_data_list()
{
    vector<string> files = get_config_files(CONFIG_DIR);
    vector<ConfigData> configs;

    for(int i = 0; i < files.size(); i++)
    {
        if(strcmp(CONFIG_DIR, "config") == 0)
        {
            ConfigData config(files[i]);
            config.set_id(i);
            config.print_config_data();
            configs.push_back(config);
        }
        else if(strcmp(CONFIG_DIR, "json") == 0)
        {
            ConfigData config;
            string filename = fs::path(files[i]).filename().string();
            config.collect_json_data(config.read_json(filename));
            config.set_id(i);
            config.print_config_data();
            configs.push_back(config);
        }
    }

    return configs;
}

int main()
{
    vector<ConfigData> configs = config_data_list();

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
