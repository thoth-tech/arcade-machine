#include <iostream>
#include <string>
#include <vector>
#include "splashkit.h"
#include "Database.h"
#include <tuple>

string TABLE_NAME = "gameData";

class GameData {
    private:
        string m_gameName;
        int m_startTime;
        int m_endTime;
        int m_rating;
        int m_highScore;

    public:
        GameData(){}

        // Constructor
        GameData(string gameName, int startTime, int endTime, int rating, int highScore) {
            m_gameName = gameName;
            m_startTime = startTime;
            m_endTime = endTime;
            m_rating = rating;
            m_highScore = highScore;
        };
        // Destructor
        ~GameData(){};

        // Setters
        void setGameName(string gameName) { m_gameName = gameName; }
        void setStartTime(int start_time) {m_startTime = start_time;}
        void setEndTime(int end_time) {m_endTime = end_time;}
        void setRating(int rating) {m_rating = rating;}
        void setHighScore(int high_score) {m_highScore = high_score;}

        // Getters
        string getGameName() {return m_gameName;}
        int getStartTime() {return m_startTime;}
        int getEndTime() {return m_endTime;}
        int getRating() {return m_rating;}
        int getHighScore() {return m_highScore;}


        // Creates a new table if it doesn't exist
        // Writes the objects data to the table

        // Useage:
        // GameData data = GameData("gameName", startTime, endTime, rating, highScore);
        // data.writeData(db);
        void writeData(Database *db) {
            // Create a new row

            std::map<std::string, std::string> row = {
                {"gameName", m_gameName},
                {"startTime", std::to_string(m_startTime)},
                {"endTime", std::to_string(m_endTime)},
                {"rating", std::to_string(m_rating)},
                {"highScore", std::to_string(m_highScore)}
            };
            // Insert the row into the table
            db->insertData(TABLE_NAME, row);

        }


        // Returns all data from the database as a vector of GameData objects

        // Useage:
        // GameData gameData = GameData();
        // vector<GameData> data = gameData.readAllGameData(db);
        vector<GameData> readAllGameData(Database *db) {
            // Get all data from the table
            std::vector<std::vector<string>> data;
            data = db->getAllData(TABLE_NAME);

            if (data.size() < 1) {
                std::cout << "No data in table" << std::endl;
                return vector<GameData>();
            }
            return formatData(data);

        }

        // enum and hash function for the swiitch statement in formatData
        enum string_code {
            eGameName,
            eStartTime,
            eEndTime,
            eRating,
            eHighScore
        };

        string_code hashit (std::string const& inString) {
            if (inString == "gameName") {
                return eGameName;
            } else if (inString == "startTime") {
                return eStartTime;
            } else if (inString == "endTime") {
                return eEndTime;
            } else if (inString == "rating") {
                return eRating;
            } else if (inString == "highScore") {
                return eHighScore;
            } else {
                return eGameName;
            }
        }

        // Returns all data specific to a game from the database as a vector of GameData objects

        // Useage:
        // GameData gameData = GameData();
        // vector<GameData> data = gameData.readGameData(db);
        std::vector<GameData> readGameData(Database *db) {
            // Get all data from the table
            std::vector<std::vector<string>> data;

            std::map<std::string, std::string> where = {
                {"gameName", m_gameName}
            };
            data = db->getData(TABLE_NAME, where);

            if (data.size() < 1) {
                std::cout << "No data in table" << std::endl;
                return vector<GameData>();
            }
            return formatData(data);


        }

        // Format the data for output
        // Used in readAllGameData and readGameData
        std::vector<GameData> formatData(std::vector<std::vector<string>> data) {
            // Create a vector of GameData objects
            vector<GameData> gameData;

            std::vector<string> columnNames = data[0];

            for (int i = 1; i < data.size(); i++) {
                std::vector<string> row = data[i];
                GameData game;
                for (int j = 0; j < row.size(); j++) {
                    const char expression = hashit(columnNames[j]);
                    switch (expression) {
                        case eGameName:
                            game.setGameName(row[j]);
                            break;
                        case eStartTime:
                            game.setStartTime(std::stoi(row[j]));
                            break;
                        case eEndTime:
                            game.setEndTime(std::stoi(row[j]));
                            break;
                        case eRating:
                            game.setRating(std::stoi(row[j]));
                            break;
                        case eHighScore:
                            game.setHighScore(std::stoi(row[j]));
                            break;
                        default:
                            break;
                    }
                }
                gameData.push_back(game);
            }
            return gameData;
        }

        // Print GameData object
        void printGameData() {
            std::cout << "==== Game Data ====" << std::endl;
            std::cout << "Game Name: " << m_gameName << std::endl;
            std::cout << "Start Time: " << m_startTime << std::endl;
            std::cout << "End Time: " << m_endTime << std::endl;
            std::cout << "Rating: " << m_rating << std::endl;
            std::cout << "High Score: " << m_highScore << std::endl;
            std::cout << "==================" << std::endl;
        }

        // Returns the current row as a vector of strings
        vector<string> get_current_row_strings(query_result res) {
            vector<string> row;
            for (int i = 0; i < query_column_count(res); i++) {
                row.push_back(query_column_for_string(res, i));
            }
            return row;
        }

        // Calculates the:
        // Average rating of a vector of games
        // Total time played of a vector of games in seconds (stored in both start and end time)
        // Max high score of a vector of games
        GameData calculateStats(std::vector<GameData> data) {
            GameData stats;
            int totalRating = 0;
            int totalTime = 0;
            int maxScore = 0;
            string gameName = "";
            for (int i = 0; i < data.size(); i++) {
                gameName = data[i].getGameName();
                totalRating += data[i].getRating();
                totalTime += data[i].getEndTime() - data[i].getStartTime();
                if (data[i].getHighScore() > maxScore) {
                    maxScore = data[i].getHighScore();
                }
            }
            stats.setGameName(gameName);
            stats.setRating(totalRating / data.size());
            stats.setStartTime(totalTime);
            stats.setEndTime(totalTime);
            stats.setHighScore(maxScore);
            return stats;
        }

        std::map<std::string, GameData> calculateAllStats(std::vector<GameData> data) {
            // Returns a map of game names to GameData objects
            // Contains the average rating, total time played, and max high score for each game
            std::map<std::string, GameData> results;
            std::map<std::string, std::vector<GameData>> stats;
            for (int i = 0; i < data.size(); i++) {
                stats[data[i].getGameName()] = std::vector<GameData>();
            }
            for (int i = 0; i < data.size(); i++) {
                for (auto const& x : stats) {
                    if (x.first == data[i].getGameName()) {
                        stats[x.first].push_back(data[i]);
                    }
                }
            }
            for (auto const& x : stats) {
                results[x.first] = calculateStats(x.second);
            }
            return results;
        }

        // Print all game data
        void printAllStats(std::map<std::string, GameData> data) {
            for (auto const& x : data) {
                GameData game = x.second;
                std::cout << "==== Game Data ====" << std::endl;
                std::cout << "Game Name: " << x.first << std::endl;
                std::cout << "Start Time: " << game.getStartTime() << std::endl;
                std::cout << "End Time: " << game.getEndTime() << std::endl;
                std::cout << "Rating: " << game.getRating() << std::endl;
                std::cout << "High Score: " << game.getHighScore() << std::endl;
                std::cout << "==================" << std::endl;
            }
        }
};
