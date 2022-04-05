// ConfigData Class
// Parses the data from config text files to an object

using namespace std;
using std::vector;

class ConfigData{
    private:
        int _id;
        string _repo;
        string _language;
        string _image;
        string _title;
        string _genre;
        string _rating;
        string _author;
        string _exe;
        string _folder;
    public:
        // Default Constructor
        ConfigData(){}

        // Overloaded Constructor
        ConfigData(string config_file)
        {
            collect_config_data(read_txt(open_file(config_file)));
        }

        //Setters:
        auto set_id(int &i) { _id = i; }
        auto set_folder(string &dir) { _folder = dir; }
        // Getters:
        auto id() const -> const int& { return _id; }
        auto repo() const -> const string& { return _repo; }
        auto language() const -> const string& { return _language; }
        auto image() const -> const string& { return _image; }
        auto title() const -> const string& { return _title; }
        auto genre() const -> const string& { return _genre; }
        auto rating() const -> const string& { return _rating; }
        auto author() const -> const string& { return _author; }
        auto exe() const -> const string& { return _exe; }
        auto folder() const -> const string& { return _folder; }

        // Open a file
        ifstream open_file(string file)
        {
            ifstream config_file;

            config_file.open(file);

            if(config_file.fail())
            {
                cerr << "Error Opening File" << endl;
                exit(1);
            }

            return config_file;
        }

        vector<string> read_txt(ifstream file)
        {
            vector<string> config_items;
            string line;
            char c = '#';
            char s = ' ';

            while(getline(file, line)){
                if(line[0] != c && line[0] != s)
                {
                    config_items.push_back(line);
                }
            }
            
            return config_items;
        }

        json read_json(string filepath)
        {
            json config_items = json_from_file(filepath);
            return config_items;
        }

        void collect_json_data(json json_configs = {})
        {
            _repo = json_read_string(json_configs, "repo");
            _language = json_read_string(json_configs, "language");
            _image = json_read_string(json_configs, "image");
            _title = json_read_string(json_configs, "title");
            _genre = json_read_string(json_configs, "genre");
            _rating = json_read_string(json_configs, "rating");
            _author = json_read_string(json_configs, "author");
            _exe = json_read_string(json_configs, "exe");
        }

        void collect_config_data(vector<string> configs = vector<string>())
        {
            smatch sm;
            vector<string> data;

            if (configs.size() > 0)
            {
                for (int i = 0; i < configs.size(); i++)
                {
                    const string s = configs[i];

                    if(regex_search(s.begin(), s.end(), sm, regex(".*=(.*)")))
                        data.push_back(sm[1]);
                }

                if(!data.empty())
                {
                    _repo = data[0];
                    _language = data[1];
                    _image = data[2];
                    _title = data[3];
                    _genre = data[4];
                    _rating = data[5];
                    _author = data[6];
                    _exe = data[7];
                }
            }
        }

        bool game_exists()
        {
            bool exists = false;

            return exists;
        }

        // Clone or pull using Git commands
        void get_from_git(string url, const char* dir)
        {
            struct stat info;

            if (stat(dir, &info) != 0){
                system(("git clone " + url + " " + dir).c_str());
            } else {
                string d = dir;
                system(("git -C " + d + " pull " + url).c_str());
            }
        }

        void rename_dir(const char* dir)
        {
            string error;
            int n = _title.length();
            char name[n+1];
            strcpy(name, _title.c_str());
            try{
                rename(dir, name);
                throw(error);
            } catch(string error) {
                cerr << "Name cannot be changed" << endl;
                cerr << error << endl;
                exit(1);
            }
        }

        void delete_dir(string dir)
        {
            string error;
            try{
                system(("rmdir -s -q " + dir).c_str());
                throw(error);
            } catch (string e) {
                cerr << "Name cannot be changed" << endl;
                cerr << error << endl;
            }
        }

        void print_config_data()
        {
            string i = to_string(id());
            write_line("========================");
            write_line("ID: " + i);
            write_line("Title = " + title());
            write_line("Author = " + author());
            write_line("Genre = " + genre());
            write_line("Rating = " + rating());
            write_line("Repo = " + repo());
            write_line("Language = " + language());
            write_line("Image = " + image());
            write_line("Exe = " + exe());
            write_line("Folder = " + folder());
            write_line("========================");
        }
};