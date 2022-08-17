#ifndef ARCADE_MACHINE_DATABASE_H
#define ARCADE_MACHINE_DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include "splashkit.h"
#include "Table.h"
#include <map>
#include <tuple>

class Database {
    private:
        string m_databaseName;
        string m_databaseFileName;
        std::vector<Table*> m_tables;

    public:
        // Constructors
        Database(){
            m_databaseName = "arcadeMachine";
            m_databaseFileName = "arcadeMachine.db";
            database db = open_database(m_databaseName, m_databaseFileName);
            free_database(db);
        };

        Database(string databaseName, string databaseFileName){
            m_databaseName = databaseName;
            m_databaseFileName = databaseFileName;
            database db = open_database(m_databaseName, m_databaseFileName);
            free_database(db);
        };

        ~Database()
        {
            std::cout << "Destructor called on database: \"" << m_databaseName << "\"\n";
            std::cout << "Database: Clearing table memory...\n";
            for (auto& table : m_tables) delete table;
            m_tables.clear();
        }

        // Getters
        string getDatabaseName(){
            return m_databaseName;
        };

        string getDatabaseFileName(){
            return m_databaseFileName;
        };

        std::vector<Table*> getTables(){
            return m_tables;
        };


        // Methods

        // Creates a new table if it doesn't exist
        bool createTable(Table *table){
            database db = open_database(m_databaseName, m_databaseFileName);

            // Add the table to the Databases vector of tables
            m_tables.push_back(table);

            // Create the query
            string createTable = "CREATE TABLE IF NOT EXISTS " + table->getTableName() + " (";
            std::map<string, string> columnNames = table->getColumnNames();
            for (auto const& pair : columnNames) {
                createTable += pair.first + " " + pair.second + ", ";
            }
            createTable = createTable.substr(0, createTable.size() - 2);
            createTable += ")";

            // Execute the query
            query_result res = run_sql(db, createTable);

            free_database(db);

            // Return true if the table was created
            if (query_success(res)) {
                std::cout << "Table created successfully" << std::endl;
                free_all_query_results();
                return true;
            } else { // Return false if the table was not created
                std::cout << "Table creation failed" << std::endl;
                std::cout << createTable << std::endl;
                free_all_query_results();
                return false;
            }
        };

        // Inserts a new row into a table
        bool insertData(string tableName, std::map<string, string> data){
            // Check if table exists
            bool exists = std::get<0>(hasTable(tableName));

            if (exists) {
                database db = open_database(m_databaseName, m_databaseFileName);

                // Create the query
                string insertData = "INSERT INTO " + tableName + " (";
                for (auto const& pair : data) {
                    insertData += pair.first + ", ";
                }
                insertData = insertData.substr(0, insertData.size() - 2);
                insertData += ") VALUES (";

                for (auto const& pair : data) {
                    insertData += "'" + pair.second + "', ";
                }
                insertData = insertData.substr(0, insertData.size() - 2);
                insertData += ")";

                // Execute the query
                query_result res = run_sql(db, insertData);
                free_database(db);

                // Return true if the row was inserted
                if (query_success(res)) {
                    std::cout << "Data inserted successfully" << std::endl;
                    free_all_query_results();
                    return true;
                } else { // Return false if the row was not inserted
                    std::cout << "Data insertion failed" << std::endl;
                    std::cout << insertData << std::endl;
                    free_all_query_results();
                    return false;
                }
            } else { // Return false if the table does not exist
                std::cout << "Table does not exist" << std::endl;
                return false;
            }
        };


        // Returns a vector of vectors of strings containing the data in the table
        std::vector<std::vector<string>> getAllData(string tableName){
            // Check if table exists
            std::tuple<bool, Table*> tableExists = hasTable(tableName);

            bool exists = std::get<0>(tableExists);

            Table *table = std::get<1>(tableExists);

            if (exists) {
                std::vector<std::vector<string>> data;
                database db = open_database(m_databaseName, m_databaseFileName);

                // Create the query
                string query = "SELECT * FROM " + tableName;

                // Execute the query
                query_result res = run_sql(db, query);

                // Add the column names to the data vector first
                vector<string> columnNames;
                for (auto const& pair: table->getColumnNames()) {
                    columnNames.push_back(pair.first);
                }
                data.push_back(columnNames);

                // Add the data to the data vector
                while (has_row(res)) {
                    // Get each row as a vector of strings
                    vector<string> row = get_current_row_strings(res);

                    data.push_back(row);

                    if (!get_next_row(res)) {
                        break;
                    }
                }

                // Return the data
                if (query_success(res)) {
                    std::cout << "Data returned successfully" << std::endl;
                } else {
                    std::cout << "Data return failed" << std::endl;
                    std::cout << query << std::endl;
                }
                free_database(db);
                free_all_query_results();
                return data;

            } else { // Return an empty vector if the table does not exist
                std::cout << "Table does not exist" << std::endl;
                return std::vector<std::vector<string>>() ;
            }
        };

        // Prints the all data in the table to the console
        bool printAllData(string tableName){
            // Check if table exists
            std::tuple<bool, Table*> tableExists = hasTable(tableName);

            bool exists = std::get<0>(tableExists);

            Table *table = std::get<1>(tableExists);


            if (exists) {

                database db = open_database(m_databaseName, m_databaseFileName);

                // Create the query
                string query = "SELECT * FROM " + tableName;

                // Execute the query
                query_result res = run_sql(db, query);

                // Print the column names
                for (auto const& pair: table->getColumnNames()) {
                    std::cout << pair.first  // string (key)
                            << ':'
                            << pair.second
                            << " | "; // string's value
                }
                std::cout << std::endl;

                // Print the data
                while (has_row(res)) {
                    vector<string> row = get_current_row_strings(res);

                    for (int i = 0; i < row.size(); i++) {
                        std::cout << row[i] << " | ";
                    }
                    std::cout << std::endl;

                    if (!get_next_row(res)) {
                        break;
                    }
                }
                free_database(db);

                // Return true if the data was printed
                if (query_success(res)) {
                    std::cout << "Data printed successfully" << std::endl;
                    free_all_query_results();
                    return true;
                } else { // Return false if the data was not printed
                    std::cout << "Data print failed" << std::endl;
                    std::cout << query << std::endl;
                    free_all_query_results();
                    return false;
                }
            } else { // Return false if the table does not exist
                std::cout << "Table does not exist" << std::endl;
                return false;
            }
        };

        // Returns a bool if a row was deleted
        bool deleteData(string tableName, std::map<string, string> data){
            // Check if table exists
            bool exists = std::get<0>(hasTable(tableName));

            if (exists) {
                database db = open_database(m_databaseName, m_databaseFileName);

                // Create the query
                string query = "DELETE FROM " + tableName + " WHERE ";
                for (auto const& pair : data) {
                    query += pair.first + " = '" + pair.second + "' AND ";
                }
                query = query.substr(0, query.size() - 5);
                query += ";";

                // Execute the query
                query_result res = run_sql(db, query);


                free_database(db);

                // Return true if the row was deleted
                if (query_success(res)) {
                    std::cout << "Data deleted successfully" << std::endl;
                    free_all_query_results();
                    return true;
                } else { // Return false if the row was not deleted
                    std::cout << "Data deletion failed" << std::endl;
                    std::cout << query << std::endl;
                    free_all_query_results();
                    return false;
                }
            } else { //     Return false if the table does not exist
                std::cout << "Table does not exist" << std::endl;
                return false;
            }

        };

        // Returns a bool if a row was updated
        // The newData parameter is a map of column names to new values
        // The conditionData parameter is a map of column names to values to be used in the WHERE clause

        bool updateData(string tableName, std::map<string, string> newData, std::map<string, string> conditionData){
            // Check if table exists
            bool exists = std::get<0>(hasTable(tableName));

            if (exists) {
                database db = open_database(m_databaseName, m_databaseFileName);

                // Create the query
                string query = "UPDATE " + tableName + " SET ";
                for (auto const& pair : newData) {
                    query += pair.first + " = '" + pair.second + "', ";
                }
                query = query.substr(0, query.size() - 2);
                query += " WHERE ";
                for (auto const& pair : conditionData) {
                    query += pair.first + " = '" + pair.second + "' AND ";
                }
                query = query.substr(0, query.size() - 5);
                query += ";";

                // Execute the query
                query_result res = run_sql(db, query);

                free_database(db);
                // Return true if the row was updated
                if (query_success(res)) {
                    std::cout << "Data updated successfully" << std::endl;
                    free_all_query_results();
                    return true;
                } else { // Return false if the row was not updated
                    std::cout << "Data update failed" << std::endl;
                    std::cout << query << std::endl;
                    free_all_query_results();
                    return false;
                }
            } else {   // Return false if the table does not exist
                std::cout << "Table does not exist" << std::endl;
                return false;
            }
        };

        // Returns a bool if a table was dropped
        bool dropTable(string tableName){
            // Check if table exists
            bool exists = std::get<0>(hasTable(tableName));
            if (exists) {
                database db = open_database(m_databaseName, m_databaseFileName);
                // Create the query
                string query = "DROP TABLE " + tableName;
                // Execute the query
                query_result res = run_sql(db, query);

                free_database(db);
                if (query_success(res)) { // Return true if the table was dropped
                    std::cout << "Table dropped successfully" << std::endl;
                    free_all_query_results();
                    return true;
                } else { // Return false if the table was not dropped
                    std::cout << "Table drop failed" << std::endl;
                    std::cout << query << std::endl;
                    free_all_query_results();
                    return false;
                }
            } else { // Return false if the table does not exist
                std::cout << "Table does not exist" << std::endl;
                return false;
            }
        };


        // Returns a vector of vectors of strings containing the data matching a condition
        // The conditionData parameter is a map of column names to values to be used in the WHERE clause
        std::vector<std::vector<string>> getData(string tableName, std::map<string, string> conditionData){
            // Check if table exists
            std::tuple<bool, Table*> tableExists = hasTable(tableName);

            bool exists = std::get<0>(tableExists);

            Table *table = std::get<1>(tableExists);

            if (exists) {
                std::vector<std::vector<string>> data;
                database db = open_database(m_databaseName, m_databaseFileName);

                // Create the query
                string query = "SELECT * FROM " + tableName + " WHERE ";
                for (auto const& pair : conditionData) {
                    query += pair.first + " = '" + pair.second + "' AND ";
                }
                query = query.substr(0, query.size() - 5);
                query += ";";
                // Execute the query
                query_result res = run_sql(db, query);

                // Add the column names to the data vector first
                vector<string> columnNames;
                for (auto const& pair: table->getColumnNames()) {
                    columnNames.push_back(pair.first);
                }
                data.push_back(columnNames);

                // Add the data to the data vector
                while (has_row(res)) {
                    // Get each row as a vector of strings
                    vector<string> row = get_current_row_strings(res);

                    data.push_back(row);

                    if (!get_next_row(res)) {
                        break;
                    }
                }

                // Return the data
                if (query_success(res)) {
                    std::cout << "Data returned successfully" << std::endl;
                } else {
                    std::cout << "Data return failed" << std::endl;
                    std::cout << query << std::endl;
                }
                free_database(db);
                free_all_query_results();
                return data;
            } else { // Return an empty vector if the table does not exist
                std::cout << "Table does not exist" << std::endl;
                return std::vector<std::vector<string>>();
            }
        };

        // Returns the current row as a vector of strings
        std::vector<string> get_current_row_strings(query_result res) {
            vector<string> row;
            for (int i = 0; i < query_column_count(res); i++) {
                row.push_back(query_column_for_string(res, i));
            }
            return row;
        }

        // Returns a tuple of a bool and a table if the table exists
        std::tuple<bool, Table*> hasTable(string tableName){

            // Check if table exists
            std::tuple<bool, Table*> exists;

            for (int i= 0; i < m_tables.size(); i++){
                if (m_tables[i]->getTableName() == tableName) {
                    exists = std::make_tuple(true, m_tables[i]);
                    return exists;
                }
            }

            std::cout << "Table does not exist" << std::endl;
            exists = std::make_tuple(false, nullptr);
            return exists;
        };

        //Query a database table with a specialised query.
        //Returns query_result object.
        query_result queryDatabase(database &db, string query)
        {
            db = open_database(m_databaseName, m_databaseFileName);
            query_result res = run_sql(db, query);
            return res;
        }
};


#endif
