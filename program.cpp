#include "cdata.h"

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

    return 0;
}
