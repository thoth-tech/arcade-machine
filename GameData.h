#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string GAME_DATA_PATH = "data/gameData.csv";

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

        // Write data to a csv file
        void writeData(){
            ofstream myFile;
            myFile.open(GAME_DATA_PATH, ios::app);
            myFile << m_gameName << "," << m_startTime << "," << m_endTime << "," << m_rating << "," << m_highScore << endl;
            myFile.close();
        }

        // Read data from a csv file
        // Returns a vector of GameData objects

        vector<GameData> readData(){
            ifstream myFile;
            myFile.open(GAME_DATA_PATH);

            string line;
            vector<GameData> data;

            while(getline(myFile, line, '\n')) {
                string delimiter = ",";
                vector<string> result = splitString(line, delimiter);

                string gameName = result[0];
                int startTime = stoi(result[1]);
                int endTime = stoi(result[2]);
                int rating = stoi(result[3]);
                int highScore = stoi(result[4]);

                data.push_back(GameData(gameName, startTime, endTime, rating, highScore));
            }
            return data;
        }

        // Split a string into a vector of strings using a delimiter
        vector<string> splitString (string input, string delimiter) {
            size_t startPosition = 0, endPosition, delimiterLength = delimiter.length();
            string token;
            vector<string> result;

            while ((endPosition = input.find (delimiter, startPosition)) != string::npos) {
                token = input.substr (startPosition, endPosition - startPosition);
                startPosition = endPosition + delimiterLength;
                result.push_back (token);
            }

            result.push_back (input.substr (startPosition));
            return result;
        }

};
