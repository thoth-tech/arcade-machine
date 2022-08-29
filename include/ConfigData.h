#ifndef ARCADE_MACHINE_CONFIG_DATA_H
#define ARCADE_MACHINE_CONFIG_DATA_H

#include <vector>
#include <fstream>
#include <cstring>
#include <experimental/filesystem>
#include "splashkit.h"

/**
* @brief Parses the configuration data from config.txt files to a data object
* 
*/
class ConfigData
{
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
    ConfigData() {}
    ConfigData(std::string configFile);

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

    std::ifstream openFile(std::string file);
    std::vector<std::string> readTxt(std::ifstream file);
    void collectConfigData(std::vector<std::string> configs = std::vector<std::string>());
    json readJson(std::string filepath);
    void collectJsonData(json json_configs = {});
    bool getFromGit(std::string url, const char* dir);
    void renameDir(const char* dir);
    void deleteDir(std::string dir);
    void printConfigData();

};

#endif
