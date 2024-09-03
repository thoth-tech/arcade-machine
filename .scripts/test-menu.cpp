#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <map>
#include <unistd.h>
#include <termios.h>

using namespace std;
namespace fs = std::filesystem;

const string GIT_URL = "https://github.com";
const string GAME_BINARY_NAME = "DEBUG_GAME_BUILD";
const string CWD = fs::current_path();
const int STRING_MAX_SIZE = 100; //Cap input size to prevent overflows

//Handle terminal colours with escape codes
const string TERM_TEXT_RED = "\033[31m";
const string TERM_TEXT_WHITE = "\033[37m";

//Default bindings are taken from Thoth Tech Arcade Machine Operation doc
enum INPUT_BINDS{
    INPUT_KEY_UP =     65,  //KB up arrow (joystick)
    INPUT_KEY_DOWN =   66,  //KB down arrow
    INPUT_KEY_LEFT =   68,  //KB left arrow
    INPUT_KEY_RIGHT =  67,  //KB right arrow
    INPUT_KEY_SELECT = 'z', //P1 Button 5
    INPUT_KEY_BACK =   'x', //P1 Button 6
    INPUT_KEY_QUIT =   'c', //P1 Button 7
};

//Arcade uses joy to key bindings, give user actually helpful bindings
map<int, string> INPUT_ARCADE_MAP{
    {INPUT_KEY_UP,     "Joy X+"},
    {INPUT_KEY_DOWN,   "Joy X-"},
    {INPUT_KEY_LEFT,   "Joy Y-"},
    {INPUT_KEY_RIGHT,  "Joy Y+"},
    {INPUT_KEY_SELECT, "P1 Button 5"},
    {INPUT_KEY_BACK,   "P1 Button 6"},
    {INPUT_KEY_QUIT,   "P1 Button 7"},
};

//Prevent users form entering non-standard ascii codes (control codes etc)
enum ASCII_CHAR_RANGE{
    ASCII_CHAR_MIN = 32,
    ASCII_CHAR_MAX = 127
};

string MENU_ITEMS[] = {
    "Auto build",
    "Print config file",
    "Modify config file",
    "Sync repo",
    "Compile game",
    "Run game",
    "View keybindings",
    "Cleanup temporary folders",
    "Exit"
};

unsigned char get_key_press(){
    unsigned char c;
    scanf("%c", &c);
    //If user pressed a hit a key then clear the screen
    if(c != 0){
        std::system("clear");
    }
    return c;
}

void setup_filestructure(string dir){
    printf("Creating up files in: %s\n", dir.c_str());
    //Clean slate for each test
    if(fs::is_directory(dir)){
        fs::remove_all(dir);
    }
    fs::create_directory(dir.c_str());
}

void cleanup_filestructure(map<string, string> &conf){
    string dir = CWD + "/" + conf["tempDir"];
    printf("Cleaning up files in: %s\n", dir.c_str());
    fs::remove_all(dir);
    fs::remove(conf["arcadeLaunchDir"]+"/DebugGameLauncher.sh");
}

char get_next_ascii_val(char c, char input){
    if(input == INPUT_KEY_UP && c < ASCII_CHAR_MAX){
        return c + 1;
    }
    else if(input == INPUT_KEY_DOWN && c > ASCII_CHAR_MIN){
        return c - 1;
    }
    else{
        return c;
    }
}

void traverse_string(const string &s, int &pos, char input){
    if(input == INPUT_KEY_RIGHT && pos < s.size()-1){
        pos += 1;
    }
    else if(input == INPUT_KEY_LEFT && pos > 0){
        pos -= 1;
    }
}

string get_string_from_user(string message, string sin){
    int pos = 0;
    char c = sin[0];
    unsigned char kbInput;
    string padding = "";

    while(kbInput != INPUT_KEY_QUIT){
        padding.resize(pos);
        std::fill(padding.begin(), padding.end(), ' ');
        printf("%s\n", message.c_str());
        printf("%s^\n", padding.c_str());
        printf("%s      pos:%c %d\n", sin.c_str(), sin[pos], pos);
        printf("%sv\n", padding.c_str());
        printf("Backspace: %s Return: %s\n", INPUT_ARCADE_MAP[INPUT_KEY_BACK].c_str(), INPUT_ARCADE_MAP[INPUT_KEY_QUIT].c_str());
        kbInput = get_key_press();
        //Change selected character
        if(kbInput == INPUT_KEY_UP || kbInput == INPUT_KEY_DOWN){
            c = get_next_ascii_val(c, kbInput);
            sin[pos] = c;
        }
        //Move backwards & forwards
        else if(kbInput == INPUT_KEY_LEFT || kbInput == INPUT_KEY_RIGHT && pos != sin.size()-1){
            traverse_string(sin, pos, kbInput);
            //match with underlying character in string for easier text updating
            c = sin[pos];
        }
        //Add new char if user is at end of string
        else if(kbInput == INPUT_KEY_RIGHT && pos == sin.size()-1 && sin.size() < STRING_MAX_SIZE){
            sin.push_back('_');
            pos = sin.size()-1;
        }
        //Delete last char
        else if(kbInput == INPUT_KEY_BACK){
            sin.pop_back();
            pos = sin.size()-1;
        }
    }
    return sin;
}

string set_repo_url(string url, string uname, string repo){
    string out; 
    out += url + "/" + uname + "/" + repo + ".git";
    printf("Repo URL: %s\n", out.c_str());
    return out;
}

void clone_repo_to(string repo, string dir){
    auto p = fs::current_path();
    fs::current_path(p /= dir);
    
    // //Doesnt sanitize input
    string command = "git clone ";
    command+= + repo.c_str();
    system(command.c_str());

    fs::current_path(CWD);
}

auto init_config_items(){
    map<string, string> m = {
        {"tempDir", ""},
        {"repoUrl", ""},
        {"gitUserName", ""},
        {"gitRepoName", ""},
        {"arcadeLaunchDir", "/home/will/Games/LaunchScripts"},
        {"compileCmd", "skm clang++ *.cpp -o " + GAME_BINARY_NAME}
    };
    return m;
}

void read_config_from_file(map<string, string> &c){
    std::ifstream cFile ("config.cfg");
    if (cFile.is_open()){
        std::string line;

        while(getline(cFile, line)){
            auto delimiterPos = line.find("=");
            string name = line.substr(0, delimiterPos);
            string value = line.substr(delimiterPos + 1);

            //Test to see if any keys match
            if(c.find(name.c_str()) == c.end()){
                continue;
            }
            else{
                c[name.c_str()] = value.c_str();
            }
        }
    }
    else{
        throw std::runtime_error("Couldn't open config file for reading.");
    }
    c["repoUrl"] = set_repo_url(GIT_URL, c["gitUserName"], c["gitRepoName"]);
}

void print_config(map<string, string> conf){
    std::map<std::string, string>::iterator iterator = conf.begin();
    printf("!!WARNING!!\nDo not modify the name of output binary. This will break scripting!\n\n");
    while(iterator != conf.end()){
        printf("%s=%s\n", iterator->first.c_str(), iterator->second.c_str());
        ++iterator;
    }
    printf("\n");
}

void save_config_to_file(map<string, string> conf){
    std::map<std::string, string>::iterator iterator = conf.begin();
    string text = "";
    printf("Saving config file.....\n");
    while(iterator != conf.end()){
        text += iterator->first + "=" + iterator->second + "\n";
        ++iterator;
    }

    //Write out to file
    fs::current_path(CWD);
    ofstream configFile;
    configFile.open("config.cfg");
    configFile << text.c_str();
    configFile.close();
}

void update_config(map<string, string> &conf){
    //Select item to update
    std::map<std::string, string>::iterator iterator = conf.begin();
    unsigned char kbInput;
    int count = 0; //Iterator runs out of range sometimes (new line in config?) counter prevents this
    while(kbInput != INPUT_KEY_QUIT){
        printf("===========================================\n");
        print_config(conf);
        printf("===========================================\n");
        printf("%s%s=%s%s\n", TERM_TEXT_RED.c_str(), iterator->first.c_str(), iterator->second.c_str(), TERM_TEXT_WHITE.c_str());
        printf("Select option with %s, Press %s key to return\n", INPUT_ARCADE_MAP[INPUT_KEY_SELECT].c_str(), INPUT_ARCADE_MAP[INPUT_KEY_QUIT].c_str());
        kbInput = get_key_press();
        if(kbInput == INPUT_KEY_UP && count > 0){
            --iterator; 
            --count;
        }
        else if(kbInput == INPUT_KEY_DOWN && count < conf.size()-1){
            ++iterator;
            ++count;
        }
        else if(kbInput == INPUT_KEY_SELECT){
            conf[iterator->first] =  get_string_from_user("Enter new config value:", iterator->second);
        }
    }

    //Once user has quit prompt them to save the new config to file
    printf("Save updated config?\n");
    printf("Yes:%s No:%s\n", INPUT_ARCADE_MAP[INPUT_KEY_SELECT].c_str(), INPUT_ARCADE_MAP[INPUT_KEY_BACK].c_str());
    kbInput = get_key_press();
    if(kbInput == INPUT_KEY_SELECT){
        save_config_to_file(conf);
    }
    //Re-read in the new config file, reset config if user did not save
    read_config_from_file(conf);
}

void compile_game(string path, string compileCmd){
    //Assume that project has been downloaded to folder of repo's name
    fs::current_path(CWD);
    auto p = fs::current_path();
    fs::current_path(p /= path);
    printf("Path: %s\n", p.c_str());
    printf("Compiling game using: \"%s\"\n", compileCmd.c_str());
    system(compileCmd.c_str());
}

void create_game_launcher(map<string, string> &c){
    string cmd;
    fs::current_path(CWD);
    //Symlinks do not work due to resource dependencies. Use shell script instead
    system("touch DebugGameLauncher.sh");
    system("chmod +x DebugGameLauncher.sh");

    //Setup vars for shell script
    cmd += "#!/bin/sh\n";
    cmd += "cd \"" + CWD + "/" + c["tempDir"] + "/" + c["gitRepoName"] + "\"\n";
    cmd += "./" + GAME_BINARY_NAME;

    //Write out to file
    ofstream launcherScript;
    launcherScript.open("DebugGameLauncher.sh");
    launcherScript << cmd.c_str();
    launcherScript.close();
    printf("Script: %s\n", cmd.c_str());

    //Move the launcher to the arcade games folder
    cmd = "mv " + CWD + "/DebugGameLauncher.sh " + c["arcadeLaunchDir"];
    printf("Moving game launcher using\n%s\n", cmd.c_str());
    system(cmd.c_str());
}

void print_input_map(){
    //Kind of useless are arcade uses an opaque joy mapping, included for debugging
    printf("Note: Arcade cabinet uses joy-to-key bindings for input\n");
    printf("Up: %c, Down: %c, Left: %c, Right: %c, Select: %c, Back: %c, Quit: %c,\n", 
        INPUT_KEY_UP, INPUT_KEY_DOWN, INPUT_KEY_LEFT, INPUT_KEY_RIGHT, INPUT_KEY_SELECT, INPUT_KEY_BACK, INPUT_KEY_QUIT);
    printf("Up: %s, Down: %s, Left: %s, Right: %s, Select: %s, Back: %s, Quit: %s\n\n", 
        INPUT_ARCADE_MAP[INPUT_KEY_UP].c_str(), INPUT_ARCADE_MAP[INPUT_KEY_DOWN].c_str(), INPUT_ARCADE_MAP[INPUT_KEY_LEFT].c_str(), 
        INPUT_ARCADE_MAP[INPUT_KEY_RIGHT].c_str(), INPUT_ARCADE_MAP[INPUT_KEY_SELECT].c_str(), INPUT_ARCADE_MAP[INPUT_KEY_BACK].c_str(), INPUT_ARCADE_MAP[INPUT_KEY_QUIT].c_str());
}

void disable_term_buffer(termios config){
    struct termios newTerm;
    newTerm = config;
	
    //Disable buffer and turn echo off
	newTerm.c_lflag &=(~ICANON & ~ECHO);
    printf("\e[?25l"); //Hide cursor, Termios doens't have a flag for this?

	//Immediately apply new config
	tcsetattr(STDIN_FILENO,TCSANOW,&newTerm);
}

void enable_term_buffer(termios tio){
	tcsetattr(STDIN_FILENO,TCSANOW,&tio);
    printf("\e[?25h"); //Show cursor
}

void print_main_menu(int selection){
    for (int i = 0; i < sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0]); i++){
        if(selection == i){
            //Print highlighted line red
            printf("%s%s  <--%s\n", TERM_TEXT_RED.c_str(), MENU_ITEMS[i].c_str(), TERM_TEXT_WHITE.c_str());
        }
        else{
            printf("%s\n", MENU_ITEMS[i].c_str());
        }
    }
}

void main_menu(map<string, string> conf){
    int selection = 0;
    unsigned char kbInput;
    while(kbInput != INPUT_KEY_QUIT){
        print_main_menu(selection);
        printf("Select: %s, Quit: %s\n", INPUT_ARCADE_MAP[INPUT_KEY_SELECT].c_str(), INPUT_ARCADE_MAP[INPUT_KEY_QUIT].c_str());
        kbInput = get_key_press();
        //Get user selection
        if(kbInput == INPUT_KEY_UP && selection > 0){
            selection -= 1;
        }
        else if(kbInput == INPUT_KEY_DOWN && selection < sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0])-1){
            selection += 1;
        }
        
        //Execute chosen selection
        if(kbInput == INPUT_KEY_SELECT){
            switch (selection){
                case 0: //Auto build
                    setup_filestructure(conf["tempDir"]);
                    clone_repo_to(conf["repoUrl"], conf["tempDir"]);
                    compile_game((conf["tempDir"] + "/" + conf["gitRepoName"]), conf["compileCmd"]);
                    create_game_launcher(conf);
                    break;
                
                case 1: //Print config file
                    while(kbInput != INPUT_KEY_QUIT){
                        print_config(conf);
                        printf("Press %s to return:\n", INPUT_ARCADE_MAP[INPUT_KEY_QUIT].c_str());
                        kbInput = get_key_press();
                    }
                    kbInput = 0; //reset or program immediately exits
                    break;

                case 2: //Modify config file
                    update_config(conf);
                    break;

                case 3: //Sync repo
                    setup_filestructure(conf["tempDir"]);
                    clone_repo_to(conf["repoUrl"], conf["tempDir"]);
                    break;

                case 4: //Compile game
                    try{
                        compile_game((conf["tempDir"] + "/" + conf["gitRepoName"]), conf["compileCmd"]);
                        create_game_launcher(conf);    
                    }
                    catch(const std::exception& e){
                        cerr << e.what() << '\n';
                        printf("%sResync repo & try again%s\n", TERM_TEXT_RED.c_str(), TERM_TEXT_WHITE.c_str());
                    }
                    break;

                case 5: //Run game
                    fs::current_path(conf["arcadeLaunchDir"]);
                    system("./DebugGameLauncher.sh");
                    fs::current_path(CWD);
                    break;

                case 6: //View keybinds
                    print_input_map();
                    break;

                case 7: //Cleanup temporary folders
                    cleanup_filestructure(conf);
                    break;

                case 8: //Exit
                    return;

                default:
                    break;
            }
        }        
    }
}

int main(){
    //Setup terminal 
    struct termios defaultTerm;
    tcgetattr(STDIN_FILENO,&defaultTerm);
    disable_term_buffer(defaultTerm);
    
    //Setup config
    auto conf = init_config_items();
    read_config_from_file(conf);
    
    main_menu(conf);

    //Reset terminal
    enable_term_buffer(defaultTerm);
    return 0;
}
