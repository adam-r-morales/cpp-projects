#include "Database.h"

#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <fstream>
#include <list>
#include <vector>
#include <sstream>

#include "C:\Program Files\MySQL\Connector C++ 8.0\include\\jdbc\mysql_connection.h"
#include <C:\Program Files\MySQL\Connector C++ 8.0\include\\jdbc\cppconn\driver.h>
#include <C:\Program Files\MySQL\Connector C++ 8.0\include\\jdbc\cppconn\exception.h>
#include <C:\Program Files\MySQL\Connector C++ 8.0\include\\jdbc\cppconn\resultset.h>
#include <C:\Program Files\MySQL\Connector C++ 8.0\include\\jdbc\cppconn\statement.h>
#include <C:\Program Files\MySQL\Connector C++ 8.0\include\\jdbc\cppconn\metadata.h>
#include <C:\Program Files\MySQL\Connector C++ 8.0\include\mysqlx\xapi.h>

//Conctructor
Database::Database() : database{ "" }, table{ "" }
{
	try {
		//Initiate the driver, connection, and statement to Connector C++
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
		stmt = con->createStatement();
	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Destructor
Database::~Database()
{
	delete stmt;
	delete con;
}

//Return database name
std::string Database::getDatabase()
{
	return database;
}

//Sets up new database
bool Database::setupDatabase(std::string db_name, bool import)
{
	
	try {	
		//If "\q" is entered, return false
		if (db_name == "\\q")
			return false;
		else
		{
			//Create database
			stmt->execute("CREATE DATABASE `" + db_name + "`");

			//If this function isn't being called by import function,
			//assign new database name to class variable "database"
			if(import == false)
				database = db_name;

			std::cout << "Database \"" + db_name + "\" created and in use." << std::endl << std::endl;

			return true;
		}
	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl << std::endl;

		//If database name already exists, try a different name 
		std::cout << "Please try a different name." << std::endl << std::endl;

		std::string file_name;
		std::cout << "New File Name: ";
		std::getline(std::cin, file_name);
		if (file_name == "\\q")
		{
			return false;
		}
		setupDatabase(file_name, false);

	}
}

//Loads existing database
bool Database::loadDatabase()
{
	bool done = false;
	do {

		//Lists databases and assigns chosen database to "db"
		std::string db = dbList(stmt);

		//If "db" retruns an empty string, return false
		if (db == "")
			return false;
		else if (db == "error")     //If "db" returns "error", repeat options
			std::cout << "Invalid entry. Please try again." << std::endl;
		else
		{
			//If "db" returns database name, assign to class variable "database"
			database = db;
			std::cout << "The current database: " << database << std::endl;
			//Turn "done" to true to break out of do-while loop
			done = true;
		}

	} while (!done);

}

//Exports database to .sql file
void Database::exportDatabase(std::string db_name)
{
	
	std::ofstream batch_file;
	batch_file.exceptions(std::ofstream::badbit | std::ofstream::failbit);
	try {
		
		//Create batch file named "command.cmd"
		batch_file.open("commands.cmd", std::ios::trunc);
		//Add commands to export database to batch file
		batch_file <<
			"cd C:\\Program Files\\MySQL\\MySQL Server 8.0\\bin" << std::endl <<
			"mysqldump.exe -uroot -proot \"" << db_name + "\" > \"C:\\Users\\Adam\\Documents\\" + db_name + ".sql\"" << std::endl;
		batch_file.close();
		
		//Run batch file
		int succ = system("cmd.exe /c commands.cmd");

		//If the batch file ran successfully, print success message
		if (succ == 0)
		{
			std::cout << "The database \"" << db_name << "\" has been exported to the file \"" <<
				db_name << ".sql\" successfully." << std::endl << std::endl;
		}
		else //If th batch file was not run successfully,
		{
			//delete newly created .sql file
			deleteFile(db_name);
			//Allow user to try again
			std::cout << "File \"" << db_name << "\" cannot be exported because it does not exist. " <<
				"Please try again.\n" << std::endl;
			std::cout << "File Name: ";
			std::getline(std::cin, db_name);
			if(db_name != "\\q")
				exportDatabase(db_name);
		}
	}
	catch (std::ofstream::failure& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
	
}

//Import database from .sql file
bool Database::importDatabase(std::string db_name, std::string new_name = NULL)
{
	std::ofstream batch_file;
	batch_file.exceptions(std::ofstream::badbit | std::ofstream::failbit);
	try {
		
		//If the import function is being used to import database,
		if (database.empty())
		{
			//If quitting, return false
			if (!setupDatabase(db_name, true))
			{
				return false;
			}
			//create batch file named "commands.cmd"
			std::ofstream batch_file;
			batch_file.open("commands.cmd", std::ios::trunc);
			//Add commands to import database to "commands.cmd"
			batch_file <<
				"cd C:\\Program Files\\MySQL\\MySQL Server 8.0\\bin" << std::endl <<
				"mysql.exe -uroot -proot \"" << db_name + "\" < \"C:\\Users\\Adam\\Documents\\" + db_name + ".sql\"" << std::endl;
			batch_file.close();
		}
		else  //If the import function is being used to rename database,
		{
			//If quitting, return false
			if (!setupDatabase(db_name, true))
			{
				return false;
			}
			//create batch file named "commands.cmd"
			batch_file.open("commands.cmd", std::ios::trunc);
			//Add commands to "commands.cmd"
			batch_file <<
				"cd C:\\Program Files\\MySQL\\MySQL Server 8.0\\bin" << std::endl <<
				"mysql.exe -uroot -proot \"" << new_name + "\" < \"C:\\Users\\Adam\\Documents\\" + db_name + ".sql\"" << std::endl;
			batch_file.close();
		}
		//Run batch file
		int succ = system("cmd.exe /c commands.cmd");

		//If bacth file ran successfully and function is being used to import
		//database, assign imported database name to class variable "database"
		if (succ == 0 && database.empty())
		{
			database = db_name;
			std::cout << "The database \"" << database << "\" has been imported from the file \"" <<
				db_name << ".sql\" successfully and is in use." << std::endl << std::endl;
			
		}
		else if (succ > 0)    //If bacth file did not run successfully, 
		{
			//Delete newly created database
			stmt->execute("DROP DATABASE `" + db_name + "`");
			//Allow user to try again
			std::cout << "File \"" << db_name << "\" cannot be imported because it does not exist. " <<
				"Please try again.\n" << std::endl;
			std::cout << "File Name(without format tag): ";
			std::getline(std::cin, db_name);
			if (!importDatabase(db_name, ""))
				return false;
		}
		
		return true;
	}
	catch (std::ofstream::failure& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;

	}
}

//Renames database
bool Database::renameDatabase(std::string new_name)
{
	//If "\q" is entered, return false
	if (new_name == "\\q")
		return false;

	//exports current database to .sql file
	exportDatabase(database);
	//Deletes existing database
	dropDatabase();
	//imports current database from .sql file to database "new_name"
	importDatabase(database, new_name);

	std::ofstream batch_file;
	batch_file.exceptions(std::ofstream::badbit | std::ofstream::failbit);
	try {
		
		//Calls deleteFile function to delete uneeded .sql file
		int succ = deleteFile(database);

		//If deletion was successful, print success message
		if (succ == 0)
		{
			std::cout << "The file named " << database << " has been successfully renamed " <<
				new_name << std::endl;
		}

		//Reassign "new_name" to "database"
		database = new_name;

		return true;

	}
	catch (std::ofstream::failure& e) {
		std::cout << "***Error*** " << e.what() << std::endl;
	}
}

//Deletes database
void Database::dropDatabase()
{
	try {
		//Delete database
		stmt->execute("DROP DATABASE `" + database + "`");

		std::cout << "The file named " << database << " has been successfully deleted.\n" << std::endl;

		//Asks user to select another database to work in
		std::cout << "Since the current file has been deleted, a new file needs to be picked.\n" <<
			std::endl;

		//Calls loadDatabase function
		loadDatabase();

	}
	catch (sql::SQLException& e) {
		std::cout << "***Error*** " << e.what() << std::endl;
	}
}

//Returns name of current table
std::string Database::getTable()
{
	return table;
}

//Sets up new table
bool Database::setupTable(std::string tbl)
{
	//If "\q" is entered, return false
	if (tbl == "\\q")
		return false;
	try {
		//Enter database
		stmt->execute("USE `" + database + "`");
		//Create new table if it doesn't already exist
		stmt->execute("CREATE TABLE IF NOT EXISTS `" + tbl + "`( id INT )");

		//Assign "tbl" to class variable "table"
		table = tbl;

		std::cout << "Table \"" << table << "\" created and in use." << std::endl << std::endl;

		return true;

	}
	catch (sql::SQLException& e) {

		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Rename table
bool Database::renameTable()
{
	try {
		//Enter database
		stmt->execute("USE `" + database + "`");
		
		//Call tableList function and return name of selected table
		std::string tbl = tableList(stmt, database);
		std::string new_name;

		//If "tbl" is not an empty string,
		if (!tbl.empty())
		{
			//enter new table name
			std::cout << "New Table Name: ";
			std::getline(std::cin, new_name);
			std::cout << std::endl;

			//If "\q" is entered, return false
			if (new_name == "\\q")
				return false;

			//Rename "tbl" to "new_name"
			stmt->execute("RENAME TABLE `" + tbl + "` TO `" + new_name + "`");
			std::cout << "The table \"" << tbl << "\" has been renamed to \"" << new_name <<
				"\" successfully." << std::endl;

			return true;
		}
		else
		{
			std::cout << "Something went wrong. Please try again." << std::endl;
		}

	}
	catch (sql::SQLException& e) {

		std::cerr << "*** Error ***" << e.what() << std::endl;

	}
}

//Displays table
bool Database::showTable()
{

	try {
		//Enter database
		stmt->execute("USE `" + database + "`");
		
		//Call tableList function and return selected table name
		std::string tbl = tableList(stmt, database);

		//If empty string is returned from tableList function, return false
		if (tbl == "")
			return false;

		//Assign selected table to class variable "table"
		table = tbl;
		//Select count of rows in current table
		sql::ResultSet* res = stmt->executeQuery("SELECT COUNT(*) FROM `" + table + "`");
		//Select items from current table
		sql::ResultSet* rs = stmt->executeQuery("SELECT * FROM `" + table + "`");
		res->next();
		//Assign current table row count to "count"
		int count = res->getInt(1);
		float grand_total;
		std::vector<int> attr_length;

		//If there is at least one row in current table,
		if (count > 0)
		{
			//iterate through each row
			for (int i = 1; i <= count; i++)
			{
				//Select row where id equals "i"
				sql::ResultSet* res1 = stmt->executeQuery("SELECT * FROM `" + table +
					"` WHERE id = " + std::to_string(i));
				sql::ResultSetMetaData* rsmd = res1->getMetaData();
				//get column count from this row
				int col_count = rsmd->getColumnCount();
				
				//iterate through columns of this row
				for (int j = 1; j <= col_count; j++)
				{
					//Get column name
					std::string col_name = rsmd->getColumnName(j);
					int size = 0;

					//if it's the first column or the length of the column name is
					//at least as big as the max number of digits allowed in 
					//an entry,
					if (col_name.length() >= 8 || j == 1)
						//push length of column name on to "attr_length" vector
						attr_length.push_back(col_name.length());
					//If it's not the first column and the length of the column
					//name is less than the max number of digits allowed in an
					//entry,
					else if (col_name.length() < 8 && j > 1) 
					{
						//calculate difference of column name length and max number
						size = 8 - col_name.length();
						//Push modified column name length on to "attr_length" vector 
						attr_length.push_back(col_name.length() + size);
					}
					//If this is the first row,
					if (i == 1)
					{
						//Print column name
						std::cout << col_name;
						//Format spacing
						for (int l = 0; l < size; l++)
							std::cout << " ";
						//Print column border
						std::cout << "|";

						//If this is the last column, print "total"
						if (j == col_count)
						{
							std::cout << "total|";
						}
					}
					
				}
				std::cout << std::endl;

				//Call constructRow function
				constructRow(res1, attr_length, col_count);
			}
			std::cout << std::endl;
			
			//Call columnSum function and return grand total of table
			grand_total = columnSum(rs, attr_length, table);
			//Display grand total of table
			std::cout << grand_total << std::endl;
		}
		else   //If the table is empty, display this message
			std::cout << "There is nothing in this table yet." << std::endl;
		std::cout << std::endl;
		
	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Delete table
void Database::dropTable()
{
	try {
		//Enter database
		stmt->execute("USE `" + database + "`");
		//Delete current table
		stmt->execute("DROP TABLE `" + table + "`");

		std::cout << "The table named \"" << table << "\" has been successfully deleted." << std::endl;

	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Add row to table
void Database::insertRow()
{

	try {
		//Enter database
		stmt->execute("USE `" + database + "`");

		//Select row count of current table
		std::string sql = "SELECT COUNT(*) FROM `" + table + "`";
		sql::ResultSet* rs = stmt->executeQuery(sql);
		rs->next();
		//Assign row count of current table to "count"
		int count = rs->getInt(1);
		//Make id the row count plus 1
		std::string id = std::to_string(count + 1);
		
		//Insert new row with id
		stmt->execute("INSERT INTO `" + table + "`(id) VALUES (" + id + ")");

		std::cout << "Row Inserted." << std::endl;
	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}
//Add column to current table
bool Database::addColumn(std::string col)
{
	//if "\q" is entered, return false
	if (col == "\\q")
		return false;
	try {
		//Enter database
		stmt->execute("USE `" + database + "`");

		//Add column that allows a float of max 7 digits to entered
		stmt->execute("ALTER TABLE `" + table + "` ADD COLUMN `" + col + "` FLOAT(5,2)");

		std::cout << "Column " << col << " added." << std::endl;

		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Rename column
bool Database::renameColumn()
{
	try {
		//Enter database
		stmt->execute("USE `" + database + "`");

		//Select items from current table
		sql::ResultSet* res = stmt->executeQuery("SELECT * FROM `" + table + "`");
		sql::ResultSetMetaData* rsmd = res->getMetaData();
		//Get column count
		int col_count = rsmd->getColumnCount();
		int count = 0;
		std::string sel;
		size_t num;
		bool exists = false;

		do {
			std::vector<std::string> col_name;
			//Iterate through columns and push their names on to "col_name" vector
			for (int i = 2; i <= col_count; i++)
			{
				col_name.push_back(rsmd->getColumnName(i));

			}

			std::cout << "Please select the column you would like to rename from the list below:" <<
				std::endl;

			//List all column name from "col_name" vector
			for (int j = 0; j < col_name.size(); j++)
			{
				std::cout << j + 1 << ": " << col_name[j] << std::endl;
			}
			
			//Select from list
			std::cout << "Selection: ";
			if(count == 0)
				std::cin.ignore();
			std::getline(std::cin, sel);
			std::cout << std::endl;

			//If "\q" is entered, return false
			if (sel == "\\q")
				return false;

			//If selection is a digit, covert it a number
			if (std::isdigit(sel[0]))
			{
				std::stringstream sstream(sel);
				sstream >> num;
			}
			else   //If selection is not a digit, allow user to try again
			{
				std::cout << "Invalid entry. Please try again." << std::endl;
				count++;
				continue;
			}
			std::cout << std::endl;

			if (num > col_count)
			{
				std::cout << "Invalid Selection. Please try again." << std::endl;
				continue;
			}
					   			
			//Enter new column name	
			std::string new_name;
			std::cout << "New Column Name: ";
			std::getline(std::cin, new_name);
			//If "\q" is entered, return false
			if (new_name == "\\q")
				return false;
			//Change name of selected column to "new_name"
			stmt->execute("ALTER TABLE `" + table + "` RENAME COLUMN `" + col_name[num - 1] +
				"` TO `" + new_name + "`");
			std::cout << "Column " << col_name[num - 1] << " has been renamed to " <<
				new_name << "." << std::endl;
			exists = true;

			
			
			count++;
		} while (!exists);

		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}
//Update existing row
bool Database::updateRow()
{
	try {
		//Enter database
		stmt->execute("USE `" + database + "`");

		std::string id;
		std::string entry;
		std::string sql;
		std::vector<std::string> col;
		std::vector<size_t> col_num;
		std::string str;
		int num = 0;
		bool number = false;
		
		do {
			//Select row to update
			std::cout << "Row: ";
			std::cin >> id;

			//If "\q" is entered, return false
			if (id == "\\q")
				return false;
			else if (std::isdigit(id[0]))  //If a digit is entered, select row from table
			{
				sql = "SELECT * FROM `" + table +
					"` WHERE id = " + id;
			}
			else  //else, allow user to retry
			{
				std::cout << "Invalid entry. Please try again.\n" << std::endl;
				continue;
			}

			//Execute select query
			sql::ResultSet* res = stmt->executeQuery(sql);
			sql::ResultSetMetaData* rsmd = res->getMetaData();
			//Get column count
			int col_count = rsmd->getColumnCount();

			std::cout << "Would you like to update entire row or select columns?" << std::endl;
			std::cout << "1: Entire row" << std::endl;
			std::cout << "2: Select columns" << std::endl;
			
			//If the number of the column exists later on, skip this while loop
			//If it doesn't exist or first iteration through do-while loop,
			while (!number)
			{
				//Select either 1 o 2 for previous prompt
				std::cout << "Selection: ";
				std::cin >> str;

				//If "\q" is entered, return false
				if (str == "\\q")
					return false;
				else if (std::isdigit(str[0]))    //If digit is entered, 
				{
					//convert to int
					num = std::stoi(str);
				}
				else  //else, allow user to try again
				{
					std::cout << "Invalid entry. Please try again.\n" << std::endl;
					continue;
				}
			}
			
			std::cout << std::endl;

			//If "Entire row" is selected,
			if (num == 1)
			{
				//iterate through columns of row
				for (int i = 2; i <= col_count; i++)
				{
					//Get column name
					std::string col_name = rsmd->getColumnName(i);

					//Enter amount
					std::cout << "Amount for " << col_name << ": ";
					std::cin >> entry;

					// If "\q" is entered, return false
					if (entry == "\\q")
						return false;
					else if (std::isdigit(entry[0]))   //I digit is entered,
					{
						//Update data
						stmt->execute("UPDATE `" + table + "` SET `" + col_name + "` = " + entry + " WHERE id = " +
							id);
						//Turn "number" to true to skip previous while loop
						//in case of future error
						number = true;
					}
					else   //else, allow user to try again
					{
						std::cout << "Invalid entry. Please try again.\n" << std::endl;
						//Decrement "i" so that user can repeat the same row
						i--;
						number = false;
					}

				}
			}
			else if (num == 2)  //If "Selct columns" is selected,
			{
				//iterate through columns and push their name on "col" vector
				for (int i = 2; i <= col_count; i++)
				{
					col.push_back(rsmd->getColumnName(i));

				}

				std::cout << "Please select the columns you would like to update from the list below:" <<
					std::endl;

				//Create list of all columns in table
				for (int j = 0; j < col.size(); j++)
				{
					std::cout << j + 1 << ": " << col[j] << std::endl;
				}

				//If the number of the column exists later on, skip this while loop
				//If it doesn't exist or first iteration through do-while loop, 
				while (!number)
				{
					//select all of the row number that need to be updated
					std::string sels;
					std::cout << "Selection: ";
					std::cin.ignore();
					std::getline(std::cin, sels);

					//Pass selctions through stringstream
					std::stringstream s(sels);
					std::string sel;

					//While iterating through stringstream,
					while (s >> sel)
					{
						//if "\q" is entered, return false
						if (sel == "\\q")
							return false;
						else if (std::isdigit(sel[0]) != 0) //if digit is entered,
						{
							//convert selection to int
							int digit = std::stoi(sel);
							//push "digit" on "col_num" vector
							col_num.push_back(digit);
							//Turn "number" to true to skip previous while loop
							//in case of future error
							number = true;
						}
						else  //else, allow user to try again
						{
							std::cout << "Invalid entry. Please try again.\n" << std::endl;
							number = false;
						}

					}
				}

				//If "number" is set to true, meaning all column number were valid,
				if (number)
				{
					//iterate through columns
					for (int k = 0; k < col_num.size(); k++)
					{
						//Enter amount into specified clumn for row
						std::cout << "Amount for " << col[col_num[k] - 1] << ": ";
						std::cin >> entry;

						//If "\q" is entered, return false
						if (entry == "\\q")
							return false;
						else if (std::isdigit(entry[0]) != 0)  //if digit is entered,
						{
							//update data
							stmt->execute("UPDATE `" + table + "` SET `" + col[col_num[k] - 1] +
								"` = " + entry + " WHERE id = " + id);
						}
						else  //else, allow user to try again
						{
							std::cout << "Invalid entry. Please try again.\n" << std::endl;
							//Decrement "k" to repeat column
							k--;
						}
						
					}
				}
			}

			//If all data has been entered correctly, display success message, 
			if (number)
				std::cout << "Row number " << id << " has been updated with." << std::endl << std::endl;
			else   //else, display failure message
				std::cout << "Invalid entry. Please try again." << std::endl << std::endl;

			delete res;
		} while (!number);
		
		return true;
	}
	catch (sql::SQLException& e) {

		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Delete row
void Database::deleteRow(std::string id)
{
	
	try {
		//Enter database
		stmt->execute("USE `" + database + "`");
		//Delete row from current table
		stmt->execute("DELETE FROM `" + table + "` WHERE id = " + id);

		std::cout << "Row number " << id << " has been successfully deleted." << std::endl;


	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Delete column
bool Database::deleteColumn()
{
	try {
		//Enter database
		stmt->execute("USE `" + database + "`");

		//Select all item from table
		sql::ResultSet* res = stmt->executeQuery("SELECT * FROM `" + table + "`");
		sql::ResultSetMetaData* rsmd = res->getMetaData();
		//Get column count
		int col_count = rsmd->getColumnCount();
		std::vector<std::string> col;
		std::string sel;
		size_t num;
		char del;
		bool done = false;

		//Iterate through columns and push their name on to "col" vector
		for (int i = 2; i <= col_count; i++)
		{
			col.push_back(rsmd->getColumnName(i));
		}

		std::cout << "Please select the column you would like to delete from the list below:" <<
			std::endl;
		do {
			
			//Create list of columns
			for (int j = 0; j < col.size(); j++)
			{
				std::cout << j + 1 << ": " << col[j] << std::endl;
			}

			//Select column
			std::cout << "Selection: ";
			std::cin >> sel;

			//If "\q" s entered, return false
			if (sel == "\\q")
				return false;
			else if (std::isdigit(sel[0]))  //if digit is entered, convert to number
				num = std::stoi(sel);
			else
				break;
			
			//Double check with user if they want to delete the row
			std::cout << "Are you sure you would like to delete column  \"" << col[num - 1] <<
				"\"? This action cannot be undone. (y/n) ";
			std::cin >> del;

			//If user is sure,
			if (del == 'y' || del == 'Y')
			{
				//delete column
				stmt->execute("ALTER TABLE `" + table + "` DROP COLUMN `" + col[num - 1] + "`");
				//Turn "done" to true to break out of do-while loop
				done = true;
			}
			else if (del == 'n' || del == 'N')   //else, break out of do-while
				break;
			else  //els, allow user to try again
				std::cout << "Invalid entry." << std::endl;
		} while (!done);
		
		if (done)
		{
			std::cout << "Column \"" << col[num - 1] << "\" has been successfully deleted." << std::endl;
		}

	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}
//Compares the grand total sum of two tables from the same database
bool Database::compareTwoTablesSameDB()
{
	try {
		bool done = false;
		std::vector<int> filler;
		do {

			std::cout << "Which table's total would you like to compare to the " <<
				"current table?" << std::endl;
			//Call tableList function and return name of selected table
			std::string tbl = tableList(stmt, database);

			//If tableList function returns an empty string, return false
			if (tbl == "")
				return false;
			else if (tbl == "error") //if tableList returns "error", allow user to try again
			{
				std::cout << "Invalid entry. Please try again." << std::endl;
				continue;
			}

			//Select both tables from the database
			sql::ResultSet* res1 = stmt->executeQuery("SELECT * FROM `" + table + "`");
			sql::ResultSet* res2 = stmt->executeQuery("SELECT * FROM `" + tbl + "`");

			//Call columnSum function twice to calculate grand total of each table
			float total1 = columnSum(res1, filler, table);
			float total2 = columnSum(res2, filler, tbl);

			//If total from table 1 is greater than total 
			//from table 2, subtract 2 from 1
			if (total1 > total2)
			{
				std::cout << table << " = " << total1 << std::endl;
				std::cout << tbl << " = " << total2 << std::endl;
				std::cout << "Difference = " << total1 - total2 << std::endl <<
					std::endl;
				done = true;
			}
			//If total from table 1 is less than total 
			//from table 2, subtract 1 from 2
			else if (total1 < total2)   
			{
				std::cout << tbl << " = " << total2 << std::endl;
				std::cout << table << " = " << total1 << std::endl;
				std::cout << "Difference = " << total2 - total1 << std::endl <<
					std::endl;
				done = true;
			}
			else   //If they are equal, display message saying so
			{
				std::cout << table << "(" << total1 << ") = " <<
					tbl << "(" << total2 << ")" << std::endl << std::endl;
				done = true;
			}

		} while (!done);

		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Compare grand total sums of two table from different databases
bool Database::compareTwoTablesDiffDB()
{
	try {
		bool done = false;
		std::vector<int> filler;
		do {

			std::cout << "Which database would you like to enter?" << std::endl;

			//Call dbList function and return selected database name
			std::string db = dbList(stmt);

			std::cout << "Which table's total would you like to compare to the " <<
				"current table?" << std::endl;

			//Call tableList function and return selected table name
			std::string tbl = tableList(stmt, db);

			//If tableList return an empty string, return false
			if (tbl == "")
				return false;
			else if (tbl == "error")    //If tableList returns "error", allow user to try again
			{
				std::cout << "Invalid entry. Please try again." << std::endl;
				continue;
			}

			//Select table from other database
			sql::ResultSet* res2 = stmt->executeQuery("SELECT * FROM `" + tbl + "`");
			//Call columnSum function to get grand total of table 2
			float total2 = columnSum(res2, filler, tbl);

			//Re-eneter current database
			stmt->execute("USE `" + database + "`");
			//Select table from current database
			sql::ResultSet* res1 = stmt->executeQuery("SELECT * FROM `" + table + "`");
			//Call columnSum function to get grand total of table 1
			float total1 = columnSum(res1, filler, table);
			
			//If total from table 1 is greater than total 
			//from table 2, subtract 2 from 1
			if (total1 > total2)
			{
				std::cout << table << " = " << total1 << std::endl;
				std::cout << tbl << " = " << total2 << std::endl;
				std::cout << "Difference = " << total1 - total2 << std::endl <<
					std::endl;
				done = true;
			}
			//If total from table 1 is less than total 
			//from table 2, subtract 1 from 2
			else if (total1 < total2)
			{
				std::cout << tbl << " = " << total2 << std::endl;
				std::cout << table << " = " << total1 << std::endl;
				std::cout << "Difference = " << total2 - total1 << std::endl <<
					std::endl;
				done = true;
			}
			else   //If they are equal, display message saying so
			{
				std::cout << table << "(" << total1 << ") = " <<
					tbl << "(" << total2 << ")" << std::endl << std::endl;
				done = true;
			}

		} while (!done);

		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Compare two rows from same database
bool Database::compareTwoRowsSameDB()
{
	try {
		bool done = false;
		bool table_exists = true;
		bool row_exists = true;
		bool id_exists = true;
		std::vector<int> filler;
		std::string tbl;
		std::string id1;
		std::string id2;
		do {

			//If "table_exists", "row_exists", and "id_exists" are true, 
			//skip (for future error check)
			if (table_exists && row_exists && id_exists)
			{
				//Select row from current table
				std::cout << "Row from current table: ";
				std::cin >> id1;
				std::cout << std::endl;

				//If "\q" is entered, return false
				if (id1 == "\\q")
					return false;
				//If digit isn't entered, allow user to try again
				else if (!std::isdigit(id1[0]))
				{
					std::cout << "Invalid entry. Please try again.\n" << std::endl;
					continue;
				}
			}

			//If "row_exists" and "id_exists" is true, skip (for future error check)
			if (row_exists && id_exists)
			{
				
				std::cout << "Which table would you like to fetch " <<
					"the second row from?" << std::endl;

				//Call tableList function and eturn name of selected table
				tbl = tableList(stmt, database);

				//If tableList reurns empty string, return false
				if (tbl == "")
					return false;
				//If tableList returns "error, allow user to try again
				else if (tbl == "error")
				{
					std::cout << "Invalid entry. Please try again." << std::endl;
					//Turn "table_exists" to false to skip down to this section
					//and repeat
					table_exists = false;
					continue;
				}
				else   //else, "table_exists" equals true and continue
					table_exists = true;
			}

			//If "id_exists" is true, skip (for future error check)
			if (id_exists)
			{
				//Select row from second table
				std::cout << "Row from second table: ";
				std::cin >> id2;
				std::cout << std::endl;

				//If "\q" is entered, return false
				if (id2 == "\\q")
					return false;
				//If a digit isn't entered, allow user to trya again
				else if (!std::isdigit(id2[0]))
				{
					std::cout << "Invalid entry. Please try again." << std::endl;
					//Turn "row_exists" to false to skip down to this section
					//and repeat
					row_exists = false;
				}
				else   //else, "row_exists" equals true and continue
					row_exists = true;
			}
			
			//Select the row from table 1
			std::string sql1 = "SELECT * FROM `" + table + "` WHERE id = " + id1;
			sql::ResultSet* res1 = stmt->executeQuery(sql1);
			sql::ResultSetMetaData* rsmd1 = res1->getMetaData();
			//Get column count for row 1
			int col_count1 = rsmd1->getColumnCount();
			
			//Select the row from table 2
			std::string sql2 = "SELECT * FROM `" + tbl + "` WHERE id = " + id2;
			sql::ResultSet* res2 = stmt->executeQuery(sql2);
			sql::ResultSetMetaData* rsmd2 = res2->getMetaData();
			//Get column count for row 2
			int col_count2 = rsmd2->getColumnCount();
				
			//Call constructRow function and return sum of row 1
			float total1 = constructRow(res1, filler, col_count1);
			//If constructRow returns -1, 
			if (total1 == -1)
			{
				//Turn "id_exists" to false to skip down to this section
				//and repeat
				id_exists = false;
				continue;
			}
			else    //else, "id_exists" equals true and continue
				id_exists = true;
			
			//Call constructRow function and return sum of row 2
			float total2 = constructRow(res2, filler, col_count2);
			//If constructRow returns -1, 
			if (total2 == -1)
			{
				//Turn "id_exists" to false to skip down to this section
				//and repeat
				id_exists = false;
				continue;
			}
			else   //else, "id_exists" equals true and continue
				id_exists = true;
			
			//If total from row 1 is greater than total 
			//from row 2, subtract 2 from 1
			if (total1 > total2)
			{
				std::cout << "Row " << id1 << " from table "<<
					table << " = " << total1 << std::endl;
				std::cout << "Row " << id2 << " from table " <<
					tbl << " = " << total2 << std::endl;
				std::cout << "Difference = " << total1 - total2 << std::endl <<
					std::endl;
				done = true;
			}
			//If total from row 1 is less than total 
			//from row 2, subtract 1 from 2
			else if (total1 < total2)
			{
				std::cout << "Row " << id2 << " from table " <<
					tbl << " = " << total2 << std::endl;
				std::cout << "Row " << id1 << " from table " <<
					table << " = " << total1 << std::endl;
				std::cout << "Difference = " << total2 - total1 << std::endl <<
					std::endl;
				done = true;
			}
			else   //If rows 1 and 2 are equal, display message saying so
			{
				std::cout << "Row " << id1 << " from table " <<
					table << "(" << total1 << ") = " << "Row " << 
					id2 << " from table " << tbl << "(" << total2 << 
					")" << std::endl;
				done = true;
			}
			
			
		} while (!done);

		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Compare two rows from different databases
bool Database::compareTwoRowsDiffDB()
{
	try {
		bool done = false;
		bool valid_db = true;
		bool valid_tbl = true;
		bool valid_row = true;
		std::vector<int> filler;
		std::string id1;
		std::string id2;
		std::string db;
		std::string tbl;

		do {
			//If "valid_db", "valid_tbl", and "valid_row" are true, 
			//skip (for future error check)
			if (valid_db && valid_tbl && valid_row)
			{
				//Select row of current table
				std::cout << "Row from current table: ";
				std::cin >> id1;
				std::cout << std::endl;

				//If "\q" is entered, return false
				if (id1 == "\\q")
					return false;
				//If selection is not a digit, allow user to try again
				else if (!std::isdigit(id1[0]))
				{
					std::cout << "Invalid entry. Please try again." << std::endl;
					continue;
				}
			}

			//If "valid_tbl", and "valid_row" are true, 
			//skip (for future error check)
			if (valid_tbl && valid_row)
			{
				std::cout << "Which database would you like to enter?" << std::endl;

				//Call dbList function and return selected database
				db = dbList(stmt);

				//If dbList returns empty string, return false
				if (db == "")
					return false;
				//If dbList returns "error", allow user to try again 
				else if (db == "error")
				{
					std::cout << "Invalid entry. Please try again." << std::endl;
					//Turn "valid_db" to false to skip down to this section
					//and repeat
					valid_db = false;
					continue;
				}
			}

			//If "valid_row" is true, 
			//skip (for future error check)
			if (valid_row)
			{
				std::cout << "Which table would you like to get second row from?" << std::endl;

				//Call tableList function and return selected table name
				tbl = tableList(stmt, db);
			
				//If tableList returns empty string, return false
				if (tbl == "")
					return false;
				//If tableList returns "error", allow user to try again
				else if (tbl == "error")
				{
					std::cout << "Invalid entry. Please try again." << std::endl;
					//Turn "valid_tbl" to false to skip down to this section
					//and repeat
					valid_tbl = false;
					continue;
				}
			}

			//Select row from second table
			std::cout << "Row from second table: ";
			std::cin >> id2;
			std::cout << std::endl;

			//If "\q" is entered, return false
			if (id2 == "\\q")
				return false;
			//If selection is not a digit, allow user to try again
			else if (!std::isdigit(id2[0]))
			{
				std::cout << "Invalid entry. Please try again." << std::endl;
				//Turn "valid_row" to false to skip down to this section
				//and repeat
				valid_row = false;
				continue;
			}
			
			
			//Select row from the second table
			std::string sql2 = "SELECT * FROM `" + tbl + "` WHERE id = " + id2;
			sql::ResultSet* res2 = stmt->executeQuery(sql2);
			sql::ResultSetMetaData* rsmd2 = res2->getMetaData();
			//Get column count from the second table
			int col_count2 = rsmd2->getColumnCount();

			//Re-enter current database
			stmt->execute("USE `" + database + "`");
			//Select row from current table
			std::string sql1 = "SELECT * FROM `" + table + "` WHERE id = " + id1;
			sql::ResultSet* res1 = stmt->executeQuery(sql1);
			sql::ResultSetMetaData* rsmd1 = res1->getMetaData();
			//Get column count from current table
			int col_count1 = rsmd1->getColumnCount();

			//Call constructRow function and return row sum for table 1
			float total1 = constructRow(res1, filler, col_count1);
			//If constructRow returns -1, allow userr to try again
			if (total1 == -1)
			{
				std::cout << "Invalid row. Please try again." << std::endl;
				continue;
			}

			//Call constructRow function and return row sum for table 2
			float total2 = constructRow(res2, filler, col_count2);
			//If constructRow returns -1, allow userr to try again
			if (total2 == -1)
			{
				std::cout << "Invalid row. Please try again." << std::endl;
				continue;
			}

			//If total from row 1 is greater than total 
			//from row 2, subtract 2 from 1
			if (total1 > total2)
			{
				std::cout << "Row " << id1 << " from table " <<
					table << " = " << total1 << std::endl;
				std::cout << "Row " << id2 << " from table " <<
					tbl << " = " << total2 << std::endl;
				std::cout << "Difference = " << total1 - total2 << std::endl <<
					std::endl;
				done = true;
			}
			//If total from row 1 is less than total 
			//from row 2, subtract 1 from 2
			else if (total1 < total2)
			{
				std::cout << "Row " << id2 << " from table " <<
					tbl << " = " << total2 << std::endl;
				std::cout << "Row " << id1 << " from table " <<
					table << " = " << total1 << std::endl;
				std::cout << "Difference = " << total2 - total1 << std::endl <<
					std::endl;
				done = true;
			}
			else   //If rows 1 and 2 are equal, display message saying so
			{
				std::cout << "Row " << id1 << " from table " <<
					table << "(" << total1 << ") = " << "Row " <<
					id2 << " from table " << tbl << "(" << total2 <<
					")" << std::endl;
				done = true;
			}
			
		} while (!done);
			return true;
	}	
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Compare two columns from the same database
bool Database::compareTwoColumnsSameDB()
{
	try {
		//Enter database
		stmt->execute("USE `" + database + "`");

		//Select all items from current table
		sql::ResultSet* res = stmt->executeQuery("SELECT * FROM `" + table + "`");
		sql::ResultSetMetaData* rsmd = res->getMetaData();
		//Get column count of current table
		int col_count = rsmd->getColumnCount();
		std::vector<std::string> col;
		std::string sel;
		size_t col2;
		size_t col1;
		float sum1 = 0;
		float sum2 = 0;
		bool done = false;

		//Iterate through columns and push their names on to "col" vector
		for (int i = 2; i <= col_count; i++)
		{
			col.push_back(rsmd->getColumnName(i));
		}

		
		do {
			std::cout << "First column to compare (Select from list below):" <<
				std::endl;

			//Create list of collumns from current table to select from
			for (int j = 0; j < col.size(); j++)
			{
				std::cout << j + 1 << ": " << col[j] << std::endl;
			}

			//Select column
			std::cout << "Selection: ";
			std::cin >> sel;

			//If "\q" is entered, return false
			if (sel == "\\q")
				return false;
			//If selection is digit, convert string to int
			else if (std::isdigit(sel[0]))
				col1 = std::stoi(sel);
			else
				break;

			//Check to if valid column selection
			if (col1 <= col.size())
			{
				//Select column sum of column 1
				sql::ResultSet* res1 = stmt->executeQuery("SELECT SUM(`" + col[col1 - 1] +
					"`) FROM `" + table + "`");
				res1->next();
				//Get sum of column 1
				sum1 = res1->getDouble(1);
			}

			std::cout << "Select table to get second column from (Select from list below):" <<
				std::endl;

			//Call tableList function and return selected table name
			std::string tbl = tableList(stmt, database);

			//If tableList returns empty string, return false
			if (tbl == "")
				return false;

			//Select second table
			sql::ResultSet* res2 = stmt->executeQuery("SELECT * FROM `" + tbl + "`");
			sql::ResultSetMetaData* rsmd2 = res2->getMetaData();
			//Get column count for second table
			int col_count2 = rsmd2->getColumnCount();
			std::vector<std::string> other_col;

			std::cout << "Second column to compare (Select from list below):" <<
				std::endl;

			//Iterate through columns from table 2 and
			//push their names on to "other_col" vector
			for (int i = 2; i <= col_count2; i++)
			{
				other_col.push_back(rsmd2->getColumnName(i));
			}

			//Create list of columns from second table
			for (int j = 0; j < other_col.size(); j++)
			{
				std::cout << j + 1 << ": " << other_col[j] << std::endl;
			}

			//Select column
			std::cout << "Selection: ";
			std::cin >> sel;

			//If "\q" is entered, return false
			if (sel == "\\q")
				return false;
			//If selection is digit, convert string to int
			else if (std::isdigit(sel[0]))
				col2 = std::stoi(sel);
			else
				break;

			//Check if selection is valid column number
			if (col2 <= col.size())
			{
				//Select column sum
				sql::ResultSet* res1 = stmt->executeQuery("SELECT SUM(`" + other_col[col2 - 1] +
					"`) FROM `" + tbl + "`");
				res1->next();
				//Get column sum
				sum2 = res1->getDouble(1);
			}

			//If sum from column 1 is greater than total 
			//from column 2, subtract 2 from 1
			if (sum1 > sum2)
			{
				std::cout << "Column \"" << col[col1 - 1] << "\" from table " <<
					table << " = " << sum1 << std::endl;
				std::cout << "Column \"" << other_col[col2 - 1] << "\" from table " <<
					tbl << " = " << sum2 << std::endl;
				std::cout << "Difference = " << sum1 - sum2 << std::endl <<
					std::endl;
				done = true;
			}
			//If sum from column 1 is less than total 
			//from column 2, subtract 1 from 2
			else if (sum1 < sum2)
			{
				std::cout << "Column \"" << other_col[col2 - 1] << "\" from table " <<
					tbl << " = " << sum2 << std::endl;
				std::cout << "Column \"" << col[col1 - 1] << "\" from table " <<
					table << " = " << sum1 << std::endl;
				std::cout << "Difference = " << sum2 - sum1 << std::endl <<
					std::endl;
				done = true;
			}
			else   //If columns 1 and 2 are equal, display message saying so
			{
				std::cout << "Column \"" << col[col1 - 1] << "\" from table " <<
					table << "(" << sum1 << ") = " << "Column \"" <<
					other_col[col2 - 1] << "\" from table " << tbl << "(" << sum2 <<
					")" << std::endl;
				done = true;
			}
			
		} while (!done);

		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Compare two columns from different databases
bool Database::compareTwoColumnsDiffDB()
{
	try {
		//Enter database
		stmt->execute("USE `" + database + "`");

		//Select current table
		sql::ResultSet* res = stmt->executeQuery("SELECT * FROM `" + table + "`");
		sql::ResultSetMetaData* rsmd = res->getMetaData();
		//Get column count of current table
		int col_count = rsmd->getColumnCount();
		std::vector<std::string> col;
		std::string sel;
		size_t col2;
		size_t col1;
		float sum1 = 0;
		float sum2 = 0;
		bool done = false;

		//Iterate through columns of current table and
		//push their names on to "col" vector
		for (int i = 2; i <= col_count; i++)
		{
			col.push_back(rsmd->getColumnName(i));
		}


		do {
			std::cout << "First column to compare (Select from list below):" <<
				std::endl;

			//Create list of curretn table's columns
			for (int j = 0; j < col.size(); j++)
			{
				std::cout << j + 1 << ": " << col[j] << std::endl;
			}

			//Select column
			std::cout << "Selection: ";
			std::cin >> sel;

			//If "\q" is entered, return false
			if (sel == "\\q")
				return false;
			//If selection is digit, convert string to int
			else if (std::isdigit(sel[0]))
				col1 = std::stoi(sel);
			else
				break;

			//Check if valid column selection
			if (col1 <= col.size())
			{
				//Select column sum
				sql::ResultSet* res2 = stmt->executeQuery("SELECT SUM(`" + col[col1 - 1] +
					"`) FROM `" + table + "`");
				res2->next();
				//Get column sum
				sum1 = res2->getDouble(1);
			}


			std::cout << "Select second database: " << std::endl;

			//Call dbList function and return name of selected database
			std::string db = dbList(stmt);

			//If dbList reurns empty string, return false
			if (db == "")
				return false;

			//Enter other database
			stmt->execute("USE `" + db + "`");

			std::cout << "Select table to get second column from (Select from list below):" <<
				std::endl;
			
			//Call tableList function and return name of selected table
			std::string tbl = tableList(stmt, db);

			//If tableList returns empty string, return false
			if (tbl == "")
				return false;

			std::cout << "Second column to compare (Select from list below):" <<
				std::endl;

			//Select table from other database
			sql::ResultSet* res1 = stmt->executeQuery("SELECT * FROM `" + tbl + "`");
			sql::ResultSetMetaData* rsmd1 = res1->getMetaData();
			//Get column count of table
			int col_count1 = rsmd1->getColumnCount();
			std::vector<std::string> other_col;

			//Iterate through columns and push their names
			//onto "other_col" vector
			for (int i = 2; i <= col_count1; i++)
			{
				other_col.push_back(rsmd1->getColumnName(i));
			}

			//Create list of columns
			for (int j = 0; j < other_col.size(); j++)
			{
				std::cout << j + 1 << ": " << other_col[j] << std::endl;
			}

			//Select column
			std::cout << "Selection: ";
			std::cin >> sel;
			
			//If "\q" is entered, return false
			if (sel == "\\q")
				return false;
			//If selection is digit, convert string to int
			else if (std::isdigit(sel[0]))
				col2 = std::stoi(sel);
			else
				break;

			//Check if columns number is valid
			if (col2 <= other_col.size())
			{
				//Select sum of second column
				sql::ResultSet* res2 = stmt->executeQuery("SELECT SUM(`" + other_col[col2 - 1] +
					"`) FROM `" + tbl + "`");
				res2->next();
				//Get sum
				sum2 = res2->getDouble(1);
			}

			//If sum from column 1 is greater than total 
			//from column 2, subtract 2 from 1
			if (sum1 > sum2)
			{
				std::cout << "Column \"" << col[col1 - 1] << "\" from table " <<
					table << " = " << sum1 << std::endl;
				std::cout << "Column \"" << other_col[col2 - 1] << "\" from table " <<
					tbl << " = " << sum2 << std::endl;
				std::cout << "Difference = " << sum1 - sum2 << std::endl <<
					std::endl;
				done = true;
			}
			//If sum from column 1 is less than total 
			//from column 2, subtract 1 from 2
			else if (sum1 < sum2)
			{
				std::cout << "Column \"" << other_col[col2 - 1] << "\" from table " <<
					tbl << " = " << sum2 << std::endl;
				std::cout << "Column \"" << col[col1 - 1] << "\" from table " <<
					table << " = " << sum1 << std::endl;
				std::cout << "Difference = " << sum2 - sum1 << std::endl <<
					std::endl;
				done = true;
			}
			else   //If columns 1 and 2 are equal, display message saying so
			{
				std::cout << "Column \"" << col[col1 - 1] << "\" from table " <<
					table << "(" << sum1 << ") = " << "Column \"" <<
					other_col[col2 - 1] << "\" from table " << tbl << "(" << sum2 <<
					")" << std::endl;
				done = true;
			}

		} while (!done);

		return true;
	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error ***" << e.what() << std::endl;
	}
}

//Consructs a single formatted and calculates row sum
float Database::constructRow(sql::ResultSet* rs, std::vector<int> attr_length, int col_count)
{
	try {

		//Iterates through rows of table
		while (rs->next())
		{
			float sum = 0;
			//Itetrates through columns of table
			for (size_t j = 1; j <= col_count; j++)
			{
				int total_size = 0;
				//Get data from cell of table
				float data = rs->getDouble(j);
				//If method is being called from showTable method,
				if (attr_length.size() > 0)
				{
					std::string str;
					//Get length of column name
					int attr_size = attr_length[j - 1];
					//Convert data from float to string
					std::stringstream ss(str);
					ss << data;
					str = ss.str();
					//Get length of data
					int data_size = str.length();
					//Get difference of column name length and data length
					total_size = attr_size - data_size;
					//display data
					std::cout << data;
				}

				//If not the first column,
				if (j > 1)
				{
					//add data to sum of previous data
					sum = sum + data;
				}

				//If method is being called from showTable method,
				if (attr_length.size() > 0)
				{
					//Format spacing
					for (int k = 0; k < total_size; k++)
					{
						std::cout << " ";
					}
					//Print column border
					std::cout << "|";

					//If last column,
					if (j == col_count)
					{
						//print row sum
						std::cout << sum;
					}
				}
			}
			
			return sum;
		}
		
	}
	catch (sql::SQLException& e) {
		std::cout << e.what() << std::endl;

		return -1;
	}
}

//Calculates sum of column
float Database::columnSum(sql::ResultSet* res1, std::vector<int> attr_length, std::string tbl)
{

	try {

		sql::ResultSetMetaData* rsmd = res1->getMetaData();
		//Get column count
		int col_count = rsmd->getColumnCount();
		float grand_total = 0;

		
		//Iterates through columns
		for (size_t i = 1; i <= col_count; i++)
		{
			//Get column name
			std::string col_name = rsmd->getColumnName(i);
			float col_total = 0;
	
			//If this is the first column, 
			if (i == 1)
			{
				//and method is being called by showTable method,
				if (attr_length.size() > 0)
					//print
					std::cout << "=";
			}
			//If not first column
			else
			{
				//Select column sum
				sql::ResultSet* res3 = stmt->executeQuery("SELECT ROUND(SUM(`" + col_name +
					"`), 2) FROM `" + tbl + "`");

				//Iterate through columns 
				while (res3->next())
				{
					//Get column sum
					col_total = res3->getDouble(1);
					//Add column sum to grand total
					grand_total += col_total;
					//If method is being called by showTable method,
					//print column sum
					if(attr_length.size() > 0)
						std::cout << col_total;
				}
			}
			
			//If method is being called by showTable method,
			if (attr_length.size() > 0)
			{
				std::string str;
				//Get column name length
				size_t attr_size = attr_length[i - 1];
				//Convert column sum to string
				std::stringstream ss(str);
				ss << col_total;
				str = ss.str();
				//Get column cum length
				int data_size = str.length();
				//Get difference of column name length and column
				//sum length
				int total_size = attr_size - data_size;

				//Format spacing
				for (int k = 0; k < total_size; k++)
				{
					std::cout << " ";
				}
				//print column border
				std::cout << "|";
			}

		}

		return grand_total;
	}
	catch (sql::SQLException& e) {
		std::cerr << "*** Error *** " << e.what() << std::endl;
	}
}
//Creates a list of tables in the specified database
std::string Database::tableList(sql::Statement* stmt, std::string db)
{
	std::string tbl;

	try {
		//Enter database
		stmt->execute("USE `" + db + "`");

		//Execute query "show tables"
		sql::ResultSet* res = stmt->executeQuery("SHOW TABLES");


		std::vector<std::string> tbl_name;
		std::string sel;
		size_t num;
		bool exists = false;

		//Iterate through tables and push their names onto 
		//"tbl_name" vector
		while (res->next())
		{
			tbl_name.push_back(res->getString(1));
		}

		do {
			std::cout << "Please select the file from the list below: " << std::endl;

			//Creates list of tables
			for (int i = 0; i < tbl_name.size(); i++)
			{
				std::cout << i + 1 << ": " << tbl_name[i] << std::endl;
			}

			//Select table
			std::cout << "Selection: ";
			if(db == database)
				std::cin.ignore();
			std::getline(std::cin, sel);
			std::cout << std::endl;
			
			//If "\q" is entered, return empty string
			if (sel == "\\q")
				return "";

			//Convert selection from string to int
			std::stringstream sstream(sel);
			sstream >> num;
			
			//Check if selection is valid
			if (num - 1 < tbl_name.size())
			{
				//Get table name
				tbl = tbl_name[num - 1];
				exists = true;
			}
			if (!exists)
				std::cout << "Invalid selection. Please try again." << std::endl << std::endl;

		} while (!exists);

		delete res;

		return tbl;
	}
	catch (sql::SQLException& e) {

		std::cerr << "*** Error ***" << e.what() << std::endl;

		return "error";
	}
}
//Delete file
int Database::deleteFile(std::string db)
{
	std::ofstream batch_file;
	//Create batch file named "commands.cmd"
	batch_file.open("commands.cmd", std::ios::trunc);
	//Add commands to batch file
	batch_file <<
		"del C:\\Users\\Adam\\Documents\\" + db + ".sql" << std::endl;
	batch_file.close();

	//Return execution status of "command.cmd"
	return system("cmd.exe /c commands.cmd");
}

//Creates list of databases
std::string Database::dbList(sql::Statement* stmt)
{
	std::string db;

	try {
		//Execute query "show databases"
		sql::ResultSet* res = stmt->executeQuery("SHOW DATABASES");


		std::vector<std::string> db_name;
		std::string sel;
		size_t num;
		bool exists = false;

		//Iterate through databases and push their names onto
		//"db_name" vector
		while (res->next())
		{
			db_name.push_back(res->getString(1));
		}

		do {
			std::cout << "Please select the file from the list below: " << std::endl;

			//Create list of databases
			for (int i = 0; i < db_name.size(); i++)
			{
				std::cout << i + 1 << ": " << db_name[i] << std::endl;
			}

			//Select database
			std::cout << "Selection: ";
			std::cin.ignore();
			std::getline(std::cin, sel);
			std::cout << std::endl;

			//If "\q" is enetered, return empty string
			if (sel == "\\q")
				return "";

			//Convert selection from string to int
			std::stringstream sstream(sel);
			sstream >> num;

			//Check if valid selection
			if (num - 1 < db_name.size())
			{
				//Get database name
				db = db_name[num - 1];
				exists = true;
			}
			if (!exists)
				std::cout << "Invalid selection. Please try again." << std::endl << std::endl;

		} while (!exists);

		delete res;

		return db;
	}
	catch (sql::SQLException& e) {

		std::cerr << "*** Error ***" << e.what() << std::endl;

		return "error";
	}
}