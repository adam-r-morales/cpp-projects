#include <iostream>
#include <stdlib.h>

#include "Database.h"

void mainMenu(Database, char); 
void editMenu(Database, char); 
void databaseMenu(Database, char);
void tableMenu(Database, char); 
void addMenu(Database, char); 
void updateMenu(Database, char); 
void compareMenu(Database, char);



int main()
{

	

	char selection = 'a';

	Database db = Database();

	mainMenu(db, selection);
	

	return 0;
}


void mainMenu(Database db, char sel)
{
	std::string file_name;
	do {
		std::cout << "           Main Menu          " << std::endl;
		std::cout << "------------------------------" << std::endl;
		std::cout << "Please select from menu below." << std::endl;
		std::cout << "1: New File." << std::endl;
		std::cout << "2: Load File." << std::endl;
		std::cout << "3: Import File." << std::endl;
		std::cout << "4: Export File." << std::endl;
		std::cout << "Q: Quit" << std::endl;
		std::cout << "Selection: ";
		std::cin >> sel;
		std::cout << std::endl;

		switch (sel)
		{
		case '1':
			std::cout << "New File Name: ";
			std::cin.ignore();
			std::getline(std::cin, file_name);
			if (db.setupDatabase(file_name, false))
				databaseMenu(db, sel);
			else
				break;
			break;
		case '2':
			if (db.loadDatabase())
				databaseMenu(db, sel);
			else
				break;
			break;
		case '3':
			std::cout << "File Name(without format tag): ";
			std::cin.ignore();
			std::getline(std::cin, file_name);
			if (file_name == "\\q" || !db.importDatabase(file_name, ""))
			{
				break;
			}
			else
			{
				databaseMenu(db, sel);
			}
			break;
		case '4':
			std::cout << "File Name: ";
			std::cin.ignore();
			std::getline(std::cin, file_name);
			if (file_name != "\\q")
			{
				db.exportDatabase(file_name);
			}
			break;
		case 'q':
			exit(0);
			break;
		default:
			std::cout << "Invalid input. Please try again." << std::endl;
			break;
		}
	} while (true);
}

void databaseMenu(Database db, char sel)
{

	do {
		std::string file_name;

		std::cout << "         Database Menu        " << std::endl;
		std::cout << "------------------------------" << std::endl;
		std::cout << "Please select from menu below." << std::endl;
		std::cout << "1: Edit." << std::endl;
		std::cout << "2: Tables." << std::endl;
		std::cout << "3: Switch." << std::endl;
		std::cout << "4: New." << std::endl;
		std::cout << "Q: Quit" << std::endl;
		std::cout << "Selection: ";
		std::cin >> sel;
		std::cout << std::endl;

		switch (sel)
		{
		case '1':
			editMenu(db, sel);
			break;
		case '2':
			tableMenu(db, sel);
			break;
		case '3':
			db.loadDatabase();
			break;
		case '4':
			std::cout << "New File Name: ";
			std::cin >> file_name;
			db.setupDatabase(file_name, false);
			break;
		case 'q':
			exit(0);
			break;
		default:
			std::cout << "Invalid input. Please try again." << std::endl;
			break;
		}
	} while (true);
}


void editMenu(Database db, char sel)
{
	do {
		std::string file_name;
		char del;

		std::cout << "      Edit Database Menu      " << std::endl;
		std::cout << "------------------------------" << std::endl;
		std::cout << "Please select from menu below." << std::endl;
		std::cout << "1: Rename File." << std::endl;
		std::cout << "2: Delete File." << std::endl;
		std::cout << "3: Back." << std::endl;
		std::cout << "Q: Quit" << std::endl;
		std::cout << "Selection: ";
		std::cin >> sel;
		std::cout << std::endl;

		switch (sel)
		{
		case '1':
			std::cout << "New File Name: ";
			std::cin.ignore();
			std::getline(std::cin, file_name);
			if (!db.renameDatabase(file_name))
			{
				editMenu(db, sel);
			}
			else
				break;
			break;
		case '2':
			std::cout << "Are you sure you would like to delete \"" << db.getDatabase() <<
				"? This action cannot be undone. (y/n)";
			std::cin >> del;

			if (del == 'y' || del == 'Y')
				db.dropDatabase();
			else if (del == 'n' || del == 'N')
				break;
			else
				std::cout << "Invalid entry." << std::endl;
			break;
		case '3':
			databaseMenu(db, sel);
			break;
		case 'q':
			exit(0);
			break;
		default:
			std::cout << "Invalid input. Please try again." << std::endl;
			break;
		}
	} while (true);
}

void tableMenu(Database db, char sel)
{
	do {
		std::string table;
		std::string old_name;
		std::string new_name;
		char del;

		std::cout << "       Edit Table Menu        " << std::endl;
		std::cout << "------------------------------" << std::endl;
		std::cout << "Please select from menu below." << std::endl;
		std::cout << "1: Create." << std::endl;
		std::cout << "2: Rename." << std::endl;
		std::cout << "3: View/Use." << std::endl;
		std::cout << "4: Add." << std::endl;
		std::cout << "5: Update." << std::endl;
		std::cout << "6: Delete." << std::endl;
		std::cout << "7: Compare." << std::endl;
		std::cout << "8: Back." << std::endl;
		std::cout << "Q: Quit." << std::endl;
		std::cout << "Selection: ";
		std::cin >> sel;
		std::cout << std::endl;

		switch (sel)
		{
		case '1':
			std::cout << "New Table Name: ";
			std::cin.ignore();
			std::getline(std::cin, table);
			std::cout << std::endl;
			if (!db.setupTable(table))
				break;
			break;
		case '2':
			db.renameTable();
			break;
		case '3':
			db.showTable();
			break;
		case '4':
			addMenu(db, sel);
			break;
		case '5':
			updateMenu(db, sel);
			break;
		case '6':
			std::cout << "Are you sure you would like to delete \"" << db.getTable() <<
				"? This action cannot be undone. (y/n)";
			std::cin >> del;

			if (del == 'y' || del == 'Y')
				db.dropTable();
			else if (del == 'n' || del == 'N')
				break;
			else
				std::cout << "Invalid entry." << std::endl;
			break;
		case '7':
			compareMenu(db, sel);
			break;
		case '8':
			databaseMenu(db, sel);
			break;
		case 'q':
			exit(0);
			break;
		default:
			std::cout << "Invalid input. Please try again." << std::endl;
			break;
		}
	} while (true);
}

void addMenu(Database db, char sel)
{
	do {
		std::string column;
		std::string entry;
		std::string id;

		std::cout << "      Add To Table Menu       " << std::endl;
		std::cout << "------------------------------" << std::endl;
		std::cout << "Please select from menu below." << std::endl;
		std::cout << "1: Add Row." << std::endl;
		std::cout << "2: Add Column." << std::endl;
		std::cout << "3: Back." << std::endl;
		std::cout << "Selection: ";
		std::cin >> sel;
		std::cout << std::endl;

		switch (sel)
		{
		case '1':
			db.insertRow();
			break;
		case '2':
			std::cout << "Column Name: ";
			std::cin.ignore();
			std::getline(std::cin, column);
			db.addColumn(column);
			break;
		case '3':
			tableMenu(db, sel);
			break;
		default:
			std::cout << "Invalid input. Please try again." << std::endl;
			break;
		}
	} while (true);
}

void updateMenu(Database db, char sel)
{
	do {
		std::string column;
		std::string old_name;
		std::string new_name;
		std::string id;
		char del;

		std::cout << "      Update Table Menu       " << std::endl;
		std::cout << "------------------------------" << std::endl;
		std::cout << "Please select from menu below." << std::endl;
		std::cout << "1: Rename Column." << std::endl;
		std::cout << "2: Insert Data." << std::endl;
		std::cout << "3: Delete Row." << std::endl;
		std::cout << "4: Delete Column." << std::endl;
		std::cout << "5: Back" << std::endl;
		std::cout << "Selection: ";
		std::cin >> sel;
		std::cout << std::endl;

		switch (sel)
		{
		case '1':
			db.renameColumn();
			break;
		case '2':
			db.updateRow();
			break;
		case '3':
			std::cout << "Which row would you like to delete? ";
			std::cin >> id;
			std::cout << "Are you sure you would like to delete row number " << id <<
				"? This action cannot be undone. (y/n) ";
			std::cin >> del;

			if (del == 'y' || del == 'Y')
				db.deleteRow(id);
			else if (del == 'n' || del == 'N')
				break;
			else
				std::cout << "Invalid entry." << std::endl;
			break;
		case '4':
			db.deleteColumn();
			break;
		case '5':
			tableMenu(db, sel);
			break;
		default:
			std::cout << "Invalid input. Please try again." << std::endl;
			break;
		}
	} while (true);
}

void compareMenu(Database db, char sel)
{
	do {

		std::string database;
		std::string table;
		std::string column;

		std::cout << "      Compare Data Menu       " << std::endl;
		std::cout << "------------------------------" << std::endl;
		std::cout << "Please select from menu below." << std::endl;
		std::cout << "1: Compare two tables from the same database." << std::endl;
		std::cout << "2: Compare two tables from different databases." << std::endl;
		std::cout << "3: Compare two rows from the same database." << std::endl;
		std::cout << "4: Compare two rows from different databases." << std::endl;
		std::cout << "5: Compare two columns from the same database." << std::endl;
		std::cout << "6: Compare two columns from different databases." << std::endl;
		std::cout << "Q: Quit." << std::endl;
		std::cout << "Selection: ";
		std::cin >> sel;
		std::cout << std::endl;

		switch (sel)
		{
		case '1':
			db.compareTwoTablesSameDB();
			break;
		case '2':
			db.compareTwoTablesDiffDB();
			break;
		case '3':
			db.compareTwoRowsSameDB();
			break;
		case '4':
			db.compareTwoRowsDiffDB();
			break;
		case '5':
			db.compareTwoColumnsSameDB();
			break;
		case '6':
			db.compareTwoColumnsDiffDB();
			break;
		case 'q':
			exit(0);
			break;
		default:
			break;
		}
	} while (sel != 'q');
}



