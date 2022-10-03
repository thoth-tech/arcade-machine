#ifndef ARCADE_MACHINE_CONFIG_DATA_H
#define ARCADE_MACHINE_CONFIG_DATA_H

#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <cstring>

/**
 * @brief Parses the configuration data from config.txt files to a data object
 * 
 */
class ConfigData{
    private:
        /// This configs ID
        int m_id;
        /// The repository
        std::string m_repo;
        /// This programming language this game was written in 
        std::string m_language;
        /// The thumbnail image of this game  
        std::string m_image;
        /// The title of this game
        std::string m_title;
        /// The genre of this game
        std::string m_genre;
        /// The MPA classification rating of this game 
        std::string m_rating;
        /// Th author/creator of this game
        std::string m_author;
        /// The path to the executable of this game
        std::string m_exe;
        /// The folder this game is inside
        std::string m_folder;
        /// A descritpion of the game 
        std::string m_description;
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
         * @param configFile The config.txt file
         */
        ConfigData(std::string configFile)
        {
            collectConfigData(readTxt(openFile(configFile)));
        }

        //Setters:
        auto setId(int &i) { m_id = i; }
        auto setFolder(std::string &dir) { m_folder = dir; }
        // Getters:
        auto id()            const -> const int&    { return m_id;            }
        auto repo()          const -> const std::string& { return m_repo;          }
        auto language()      const -> const std::string& { return m_language;      }
        auto image()         const -> const std::string& { return m_image;         }
        auto title()         const -> const std::string& { return m_title;         }
        auto genre()         const -> const std::string& { return m_genre;         }
        auto rating()        const -> const std::string& { return m_rating;        }
        auto author()        const -> const std::string& { return m_author;        }
        auto exe()           const -> const std::string& { return m_exe;           }
        auto folder()        const -> const std::string& { return m_folder;        }
        auto description()   const -> const std::string& { return m_description;   }

        /**
         * @brief Generic open file function 
         * 
         * @param file The config.txt file
         * @return A file as ifstream object
         */
        std::ifstream openFile(std::string file)
        {
            std::ifstream configFile;

            configFile.open(file);

            if(configFile.fail())
            {
                std::cerr << "Error Opening File" << std::endl;
                exit(1);
            }

            return configFile;
        }

        /**
         * @brief Reads the contents of a file, ignoring comments indicated by '#'
         * 
         * @param file The ifstream object
         * @return An array of data from a text file
         */
        std::vector<std::string> readTxt(std::ifstream file)
        {
            std::vector<std::string> configItems;
            std::string line;
            char c = '#';
            char s = ' ';

            while(getline(file, line)){
                if(line[0] != c && line[0] != s)
                {
                    configItems.push_back(line);
                }
            }
            
            return configItems;
        }

        /**
         * @brief Populates this config data with the data from the given array
         * 
         * @param configs a vector of strings
         * @return * void
         */
        void collectConfigData(std::vector<std::string> configs = std::vector<std::string>())
        {
            std::smatch sm;

            if (configs.size() > 0)
            {
                for (int i = 0; i < configs.size(); i++)
                {
                    const std::string s = configs[i];

                    if(std::regex_search(s.begin(), s.end(), sm, std::regex("(.*)=(.*)")))
                    {
                        if      (sm[1] == "title")       this->m_title = sm[2]; 
                        else if (sm[1] == "author")      this->m_author = sm[2];
                        else if (sm[1] == "genre")       this->m_genre = sm[2];
                        else if (sm[1] == "description") this->m_description = sm[2];
                        else if (sm[1] == "rating")      this->m_rating = sm[2];
                        else if (sm[1] == "language")    this->m_language = sm[2];
                        else if (sm[1] == "image")       this->m_image = sm[2];
                        else if (sm[1] == "executable")  this->m_exe = sm[2];
                        else if (sm[1] == "repository")  this->m_repo = sm[2];
                    }
                }
            }
        }

        /**
         * @brief Parses a json filepath as string to json object
         * 
         * @param filepath 
         * @return json 
         */
        json readJson(std::string filepath)
        {
            json config_items = json_from_file(filepath);
            return config_items;
        }

        /**
         * @brief Add json strings to data object
         * 
         * @param json_configs 
         */
        void collectJsonData(json json_configs = {})
        {
            this->m_repo     = json_read_string(json_configs, "repo");
            this->m_language = json_read_string(json_configs, "language");
            this->m_image    = json_read_string(json_configs, "image");
            this->m_title    = json_read_string(json_configs, "title");
            this->m_genre    = json_read_string(json_configs, "genre");
            this->m_rating   = json_read_string(json_configs, "rating");
            this->m_author   = json_read_string(json_configs, "author");
            this->m_exe      = json_read_string(json_configs, "exe");
        }

        /**
         * @brief Clones a git repository given the URL and proposed directory name
         * 
         * @param url git repository url
         * @param dir directory to clone to
         * @return true 
         * @return false 
         */
        bool getFromGit(std::string url, const char* dir)
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
        void renameDir(const char* dir)
        {
            std::string error;
            int n = m_title.length();
            char name[n+1];
            strcpy(name, m_title.c_str());
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
        void deleteDir(std::string dir)
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
        void printConfigData()
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