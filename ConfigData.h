using namespace std;
using std::vector;

/**
 * @brief Parses the configuration data from config.txt files to a data object
 * 
 */
class ConfigData{
    private:
        /// This configs ID
        int _id;
        /// The repository
        string _repo;
        /// This programming language this game was written in 
        string _language;
        /// The thumbnail image of this game  
        string _image;
        /// The title of this game
        string _title;
        /// The genre of this game
        string _genre;
        /// The MPA classification rating of this game 
        string _rating;
        /// Th author/creator of this game
        string _author;
        /// The path to the executable of this game
        string _exe;
        /// The folder this game is inside
        string _folder;
        /// A descritpion of the game 
        string _description;
    public:
        /**
         * @brief Default Constructor
         * 
         * @return A new Config Data object
         */
        ConfigData(){}

        /**
         * @brief Construct a new Config Data object
         * 
         * @param config_file The config.txt file
         */
        ConfigData(string config_file)
        {
            collect_config_data(read_txt(open_file(config_file)));
        }

        //Setters:
        auto set_id(int &i) { _id = i; }
        auto set_folder(string &dir) { _folder = dir; }
        // Getters:
        auto id()       const -> const int&    { return _id;       }
        auto repo()     const -> const string& { return _repo;     }
        auto language() const -> const string& { return _language; }
        auto image()    const -> const string& { return _image;    }
        auto title()    const -> const string& { return _title;    }
        auto genre()    const -> const string& { return _genre;    }
        auto rating()   const -> const string& { return _rating;   }
        auto author()   const -> const string& { return _author;   }
        auto exe()      const -> const string& { return _exe;      }
        auto folder()   const -> const string& { return _folder;   }
        auto description()   const -> const string& { return _description;   }

        /**
         * @brief Generic open file function 
         * 
         * @param file The config.txt file
         * @return A file as ifstream object
         */
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

        /**
         * @brief Reads the contents of a file, ignoring comments indicated by '#'
         * 
         * @param file The ifstream object
         * @return An array of data from a text file
         */
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

        /**
         * @brief Populates this config data with the data from the given array
         * 
         * @param configs a vector of strings
         * @return * void
         */
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
                    this->_title =       data[0];
                    this->_author =      data[1];
                    this->_genre =       data[2];
                    this->_description = data[3];
                    this->_rating =      data[4];
                    this->_language =    data[5];
                    this->_image =       data[6];
                    this->_exe =         data[7];
                    this->_repo =        data[8];
                }
            }
        }

        /**
         * @brief Parses a json filepath as string to json object
         * 
         * @param filepath 
         * @return json 
         */
        json read_json(string filepath)
        {
            json config_items = json_from_file(filepath);
            return config_items;
        }

        void collect_json_data(json json_configs = {})
        {
            this->_repo = json_read_string(json_configs, "repo");
            this->_language = json_read_string(json_configs, "language");
            this->_image = json_read_string(json_configs, "image");
            this->_title = json_read_string(json_configs, "title");
            this->_genre = json_read_string(json_configs, "genre");
            this->_rating = json_read_string(json_configs, "rating");
            this->_author = json_read_string(json_configs, "author");
            this->_exe = json_read_string(json_configs, "exe");
        }

        /**
         * @brief Clones a git repository given the URL and proposed directory name
         * 
         * @param url git repository url
         * @param dir directory to clone to
         * @return true 
         * @return false 
         */
        bool get_from_git(string url, const char* dir)
        {
            struct stat info;

            if (stat(dir, &info) != 0){
                system(("git clone " + url + " " + dir).c_str());
            } else {
                string d = dir;
                system(("git -C " + d + " pull " + url).c_str());
            }

            return true;
        }

        /**
         * @brief Change the name of a directory
         * 
         * @param dir directory to change the name of
         */
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

        /**
         * @brief Delete a directory
         * 
         * @param dir name of direcotry to delete
         */
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
        
        /**
         * @brief Print the data contained in this object
         * 
         */
        void print_config_data()
        {
            string i = to_string(id());
            write_line("========================");
            write_line("ID: " + i);
            write_line("Title = " + title());
            write_line("Author = " + author());
            write_line("Genre = " + genre());
            write_line("Description = " + description());
            write_line("Rating = " + rating());
            write_line("Repo = " + repo());
            write_line("Language = " + language());
            write_line("Image = " + image());
            write_line("Exe = " + exe());
            write_line("Folder = " + folder());
            write_line("========================");
        }
};
