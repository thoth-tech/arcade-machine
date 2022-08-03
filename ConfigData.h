#ifndef ARCADE_MACHINE_CONFIG_DATA_H
#define ARCADE_MACHINE_CONFIG_DATA_H

#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <regex>
#include <fstream>

/**
 * @brief Parses the configuration data from config.txt files to a data object
 * 
 */
class ConfigData{
    private:
        /// This configs ID
        int _id;
        /// The repository
        std::string _repo;
        /// This programming language this game was written in 
        std::string _language;
        /// The thumbnail image of this game  
        std::string _image;
        /// The title of this game
        std::string _title;
        /// The genre of this game
        std::string _genre;
        /// The MPA classification rating of this game 
        std::string _rating;
        /// Th author/creator of this game
        std::string _author;
        /// The path to the executable of this game
        std::string _exe;
        /// The folder this game is inside
        std::string _folder;
        /// A descritpion of the game 
        std::string _description;
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
        ConfigData(std::string config_file)
        {
            collect_config_data(read_txt(open_file(config_file)));
        }

        //Setters:
        auto set_id(int &i) { _id = i; }
        auto set_folder(std::string &dir) { _folder = dir; }
        // Getters:
        auto id()            const -> const int&    { return _id;            }
        auto repo()          const -> const std::string& { return _repo;          }
        auto language()      const -> const std::string& { return _language;      }
        auto image()         const -> const std::string& { return _image;         }
        auto title()         const -> const std::string& { return _title;         }
        auto genre()         const -> const std::string& { return _genre;         }
        auto rating()        const -> const std::string& { return _rating;        }
        auto author()        const -> const std::string& { return _author;        }
        auto exe()           const -> const std::string& { return _exe;           }
        auto folder()        const -> const std::string& { return _folder;        }
        auto description()   const -> const std::string& { return _description;   }

        /**
         * @brief Generic open file function 
         * 
         * @param file The config.txt file
         * @return A file as ifstream object
         */
        std::ifstream open_file(std::string file)
        {
            std::ifstream config_file;

            config_file.open(file);

            if(config_file.fail())
            {
                std::cerr << "Error Opening File" << std::endl;
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
        std::vector<std::string> read_txt(std::ifstream file)
        {
            std::vector<std::string> config_items;
            std::string line;
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
        void collect_config_data(std::vector<std::string> configs = std::vector<std::string>())
        {
            std::smatch sm;
            std::vector<std::string> data;

            if (configs.size() > 0)
            {
                for (int i = 0; i < configs.size(); i++)
                {
                    const std::string s = configs[i];

                    if(std::regex_search(s.begin(), s.end(), sm, std::regex(".*=(.*)")))
                        data.push_back(sm[1]);
                }

                if(!data.empty())
                {
                    this->_title       = data[0];
                    this->_author      = data[1];
                    this->_genre       = data[2];
                    this->_description = data[3];
                    this->_rating      = data[4];
                    this->_language    = data[5];
                    this->_image       = data[6];
                    this->_exe         = data[7];
                    this->_repo        = data[8];
                }
            }
        }

        /**
         * @brief Parses a json filepath as string to json object
         * 
         * @param filepath 
         * @return json 
         */
        json read_json(std::string filepath)
        {
            json config_items = json_from_file(filepath);
            return config_items;
        }

        /**
         * @brief Add json strings to data object
         * 
         * @param json_configs 
         */
        void collect_json_data(json json_configs = {})
        {
            this->_repo     = json_read_string(json_configs, "repo");
            this->_language = json_read_string(json_configs, "language");
            this->_image    = json_read_string(json_configs, "image");
            this->_title    = json_read_string(json_configs, "title");
            this->_genre    = json_read_string(json_configs, "genre");
            this->_rating   = json_read_string(json_configs, "rating");
            this->_author   = json_read_string(json_configs, "author");
            this->_exe      = json_read_string(json_configs, "exe");
        }

        /**
         * @brief Clones a git repository given the URL and proposed directory name
         * 
         * @param url git repository url
         * @param dir directory to clone to
         * @return true 
         * @return false 
         */
        bool get_from_git(std::string url, const char* dir)
        {
            struct stat info;

            if (stat(dir, &info) != 0){
                system(("git clone " + url + " " + dir).c_str());
            } else {
                std::string d = dir;
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
            std::string error;
            int n = _title.length();
            char name[n+1];
            strcpy(name, _title.c_str());
            try{
                rename(dir, name);
                throw(error);
            } catch(std::string error) {
                std::cerr << "Name cannot be changed" << std::endl;
                std::cerr << error << std::endl;
                exit(1);
            }
        }

        /**
         * @brief Delete a directory
         * 
         * @param dir name of direcotry to delete
         */
        void delete_dir(std::string dir)
        {
            std::string error;
            try{
                system(("rmdir -s -q " + dir).c_str());
                throw(error);
            } catch (std::string e) {
                std::cerr << "Name cannot be changed" << std::endl;
                std::cerr << error << std::endl;
            }
        }
        
        /**
         * @brief Print the data contained in this object
         * 
         */
        void print_config_data()
        {
            std::string i = std::to_string(id());
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

#endif