#ifndef ARCADE_MACHINE_TABLE_H
#define ARCADE_MACHINE_TABLE_H

#include "splashkit.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Table
{
private:
    std::string m_tableName;
    std::map<string, string> m_columnNames;

public:
    Table();
    Table(string tableName)
    {
        m_tableName = tableName;
        m_columnNames = std::map<string, string>();
    };
    Table(string tableName, std::map<string, string> columnNames)
    {
        m_tableName = tableName;
        m_columnNames = columnNames;
    };

    ~Table()
    {
        std::cout << "Destructor called on table: \"" << m_tableName << "\"\n";
    }

    // Setters
    void setTableName(string tableName)
    {
        m_tableName = tableName;
    };
    void setColumnNames(std::map<string, string> columnNames)
    {
        m_columnNames = columnNames;
    };

    // Getters
    string getTableName()
    {
        return m_tableName;
    };
    std::map<string, string> getColumnNames()
    {
        return m_columnNames;
    };

    // Methods
    void addColumn(string columnName, string columnType)
    {
        m_columnNames.insert({columnName, columnType});
    };
    void removeColumn(string columnName)
    {
        m_columnNames.erase(columnName);
    };
};

#endif
