#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "splashkit.h"

using namespace std;

string DATA_BASE_NAME = "GameData";
string DATA_BASE_FILE_NAME = "gameData";
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
        // Writes the data to the table
        void writeData() {
            database db = open_database(DATA_BASE_NAME, DATA_BASE_FILE_NAME);

            string createTable = "CREATE TABLE IF NOT EXISTS " + TABLE_NAME + " (gameName TEXT, startTime INT, endTime INT, rating INT, highScore INT)";

            query_result res = run_sql(db, createTable);

            if (query_success(res)) {
                cout << "Successfully created table or table already exists" << endl;
            } else {
                cout << "Failed to create table" << endl;
            }

            string query = "INSERT INTO " + TABLE_NAME + " (gameName, startTime, endTime, rating, highScore) VALUES ('" + m_gameName + "', " + to_string(m_startTime) + ", " + to_string(m_endTime) + ", " + to_string(m_rating) + ", " + to_string(m_highScore) + ");";

            res = run_sql(DATA_BASE_NAME, query);

            if (query_success(res)) {
                cout << "Successfully inserted data" << endl;
            } else {
                cout << "Failed to insert data" << endl;
            }
            free_database(db);
        }

        // Returns all data from the database as a vector of GameData objects
        vector<GameData> readData() {
            database db = open_database(DATA_BASE_NAME, DATA_BASE_FILE_NAME);

            vector<GameData> data;

            string query = "SELECT * FROM  " + TABLE_NAME + ";";

            query_result res = run_sql(db, query);

            if (query_success(res)) {
                cout << "Successfully retrieved data" << endl;
            } else {
                cout << "Failed to retrieve data" << endl;
            }

            while (has_row(res)) {
                vector<string> row = get_current_row_strings(res);
                string gameName = row[0];
                int startTime = stoi(row[1]);
                int endTime = stoi(row[2]);
                int rating = stoi(row[3]);
                int highScore = stoi(row[4]);

                GameData newGame(gameName, startTime, endTime, rating, highScore);
                data.push_back(newGame);

                if (!get_next_row(res)) {
                    break;
                }
            }
            free_database(db);
            return data;
        }

        // Returns the current row as a vector of strings
        vector<string> get_current_row_strings(query_result res) {
            vector<string> row;
            for (int i = 0; i < query_column_count(res); i++) {
                row.push_back(query_column_for_string(res, i));
            }
            return row;
        }
};
