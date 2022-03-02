#pragma once

#include <string>
#include<vector>

#include "C:\Program Files\MySQL\Connector C++ 8.0\include\\jdbc\mysql_connection.h"
#include <C:\Program Files\MySQL\Connector C++ 8.0\include\\jdbc\cppconn\driver.h>
#include <C:\Program Files\MySQL\Connector C++ 8.0\include\\jdbc\cppconn\exception.h>
#include <C:\Program Files\MySQL\Connector C++ 8.0\include\\jdbc\cppconn\resultset.h>
#include <C:\Program Files\MySQL\Connector C++ 8.0\include\\jdbc\cppconn\statement.h>
#include <C:\Program Files\MySQL\Connector C++ 8.0\include\mysqlx\xapi.h>


class Database
{

private:
	std::string table;
	std::string database;
	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	float constructRow(sql::ResultSet*, std::vector<int>, int);
	float columnSum(sql::ResultSet*, std::vector<int>, std::string);
	std::string tableList(sql::Statement*, std::string);
	std::string dbList(sql::Statement*);
	int deleteFile(std::string);

public:
	Database();
	~Database();
	bool setupDatabase(std::string, bool);
	bool loadDatabase();
	void exportDatabase(std::string);
	bool importDatabase(std::string, std::string);
	bool renameDatabase(std::string);
	void dropDatabase();
	bool setupTable(std::string);
	bool renameTable();
	bool showTable();
	void dropTable();
	void insertRow();
	bool addColumn(std::string);
	bool renameColumn();
	bool updateRow();
	void deleteRow(std::string);
	bool deleteColumn();
	bool compareTwoTablesSameDB();
	bool compareTwoTablesDiffDB();
	bool compareTwoRowsSameDB();
	bool compareTwoRowsDiffDB();
	bool compareTwoColumnsSameDB();
	bool compareTwoColumnsDiffDB();
	std::string getTable();
	std::string getDatabase();
	
};


