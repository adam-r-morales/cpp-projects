#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>


void MainMenu();
void Login();
void ShowInbox(std::string username, std::ostream & sout);
void LoggedInMenu(std::string username);
void RegisterNewUser();
void DeleteUser();
std::string GetUserFile(std::string username);
std::string GetInboxFile(std::string username);
std::string GetLine(std::istream & sin);
bool DoesUserExist(std::string username);
int GetInboxCount(std::string username);
std::string FormatEmailString(std::string from, std::string to, std::string subject, std::string msg);
void SendEmail(std::string username);
std::string GetPassword(std::string username);


int main()
{
	MainMenu();
	return 0; // make sure your int main function returns an int
}


void MainMenu()
{
	//initialize variables
	std::string input = "";
	int menuItem = 0;
	//Formats main menu and sets parameters
	while (input != "99")
	{
		std::cout << " 1) Login" << "\n";
		std::cout << " 2) Register new user" << "\n";
		std::cout << " 3) Delete user" << "\n";
		std::cout << "99) Exit" << "\n";
		std::cout << "Selection: ";

		std::cin >> input;
		//Allows user to select options from menu
		try
		{
			menuItem = stoi(input);
			switch (menuItem)
			{
			case 1:
				Login();
				break;
			case 2:
				RegisterNewUser();
				break;
			case 3:
				DeleteUser();
				break;
			}
		}
		//Cancels action if invalid argument is entered
		catch (std::invalid_argument & ia)
		{
			std::cout << "Bad option!" << std::endl;
			std::cout << ia.what() << std::endl;
		}
	}
}
//Allows user to log in to account
void Login()
{
	//Declare variables
	std::string username;
	std::ifstream fin;
	std::string password;
	
	
	
	//User inputs username
	std::cout << "Username:";
	std::cin >> username;
	std::cout << "\n";
	//User inputs password
	std::cout << "Password:";
	std::cin >> password;
	std::cout << "\n";
	//opens user file
	fin.open(username);
	//Adds extension to username
	std::string usrnme = username + ".txt";
	
	//Initialize function variables
	std::string user = GetUserFile(username);
	std::string psswrd = GetPassword(username);
	//Compares user input to saved username and password
		if (user == usrnme && psswrd == password)
		{
			//Calls login menu
			LoggedInMenu(username);
		}
		//Cancels process if user input does not match saved items
		else
			std::cout << "No such user exists. " <<
			"Please try again or create account." << std::endl;
	
}
//Registers a new account
void RegisterNewUser()
{
	//Declare variables
	std::string usrnme;
	std::string psswrd;
	std::string username;
	
	//User input for username
		std::cout << "Enter new username: ";
		std::cin >> usrnme;
		std::cout << std::endl;
		//Opens filestream
		std::ofstream fout;
		//Creates file
		fout.open(usrnme + ".txt");
		//Sets parameters for username		
		if (usrnme.length() >= 3)
			//Saves username if parameters are met
			fout << "Username: " << usrnme << "\n";
		//Cancels action if parameters are not met
		else
		{
			//Displays error message
			std::cout << "Username must be at least 3 characters long!"
				<< std::endl;
			//Return to main menu
			MainMenu();
					
		}
				
		//User input password
		std::cout << "Enter new user password: ";
		std::cin >> psswrd;
		//Sets parameters for user input password
		if (psswrd.length() >= 6)
			//Saves password
			fout << "Password: " << psswrd;
		//Cancels action if parameters are not met
		else
		{
			//Displays errors message
			std::cout << "Password must be at least 6 characters long!"
				<< "\n";
			//Return to main menu
			MainMenu();
			
		}
		//Close file stream
		fout.close();
		//Opens filestream and creates file
		std::ofstream ofout(usrnme + "-inbox" + ".txt");
		//Saves message to file
		ofout << "#email" << "\n";
		ofout << "mailservice" << "\n";
		ofout << usrnme << "\n";
		ofout << "Intro" << "\n";
		ofout << "Hello, " << usrnme << "! " << "Welcome to out service."
			<< "\n";
		//Close file stream
		ofout.close();
		
}
//Deletes user if user exists
void DeleteUser()
{
	//Declare variables
	std::string username;
	//User chooses account to delete
	std::cout << "Who would you like to delete? ";
	std::cin >> username;
	//Initialize function variable
	std::string user = GetUserFile(username);
	//Convert string to const char pointer
	const char * name = user.c_str();
	//Initialize function variable
	std::string inbox = GetInboxFile(username);
	//Converts string to const char pointer
	const char * email = inbox.c_str();
	//Deletes user file if user exists
	if (remove(name) != 0)
		perror("Error deleting file");
	else
		puts("Account successfully deleted");
	//Deletes user inbox if user exists
	if (remove(email) != 0)
		perror("Error deleting file");
	else
		puts("Inbox successfully deleted");
}
//Sends email from one account to another
void SendEmail(std::string username)
{
	//Declare variables
	std::string user2;
	std::string subject;
	std::string message;
	

	std::string GetInboxFile(std::string username);
	//User inputs user he/she wants to send email to
	std::cout << "Send email to who: ";
	std::cin >> user2;
	std::cin.get();
	//User inputs subject of email
	std::cout << "Subject: ";
	getline(std::cin, subject);
	//User inputs message of email	
	std::cout << "Message: ";
	getline(std::cin, message);
	
	
	//Displays sending message
	std::cout << "Sending..." << std::endl;
	
	std::string formatted = FormatEmailString("From: " + username, 
		"To:" + user2, "Subject: " + subject,"Message: " + message);

	//Opens file stream
	std::fstream fin; 
	//Opens file
	fin.open(user2 + "-inbox" + ".txt", std::ios::in | std::ios::out | 
		std::ios::app);
	//Appends email to end of inbox
	fin << "#email" << "\n";
	fin << formatted;

}
//Retrieves amount of emails in inbox
int GetInboxCount(std::string username)
{
	//Opens file
	std::ifstream fin(username + "-inbox" + ".txt");
	//Declare and Initialize variables
	int count = 0;
	std::string word;
	std::string dir = "#email";
	//Scans inbox for a word
	while (fin >> word)
	{
		//Assigns "#email" to word variable
		if (word == dir)
		{
			//Increments count by 1 everytime "#email" is found in inbox
			count++;
		}
	}
	//Close file
	fin.close();
	//Returns number of emails in inbox
	return count;
}
//Diplays login menu
void LoggedInMenu(std::string username)
{
	//Initialize variables
	int selection = 0;

	std::string input = "";


	//Diplays menu while parameters is true
	while (input != "99")
	{
		std::cout << " 1) Inbox[" << GetInboxCount(username) << "]" << "\n";
		std::cout << " 2) Send Email" << "\n";
		std::cout << "99) Logout" << "\n";
		std::cout << "Selection: ";

		std::cin >> input;
		//Allows user to select options
		try
		{
			selection = stoi(input);
			switch (selection)
			{
			case 1:
				std::cout;
				ShowInbox(username, std::cout);
				break;
			case 2:
				SendEmail(username);
				break;
			case '\n': 
			case '\t':
			case ' ':
				break;
			

			}
		}
		//Cancels action if invalid argument is entered
			catch (std::invalid_argument & ia)
			{
				//Displays error message
				std::cout << "Invalid selection! Please try again." << std::endl;
				
			}
		

	}
}
//Gets Password
std::string GetPassword(std::string username)
{
	//Opens file
	std::ifstream fin(username + ".txt");
	//Initialize variables
	std::string user;
	std::string passkey;
	//Skips first line
	std::getline(fin, user);
	//Skips first word of second line
	fin >> passkey;
	
	//Initialize function variable
	std::string psswrd = GetLine(fin);
	//Erases space before password
	psswrd.erase(psswrd.begin());
	//Close file
	fin.close();
	//Returns password
	return psswrd;
		
}
//Shows contents of inbox
void ShowInbox(std::string username, std::ostream & sout)
{
	//Opens file
	std::string str1 = GetInboxFile(username);
	//Initialize variable
	std::string str;

	std::ifstream fin(username + "-inbox.txt");

	
	//If file is open, continue
			
		//While it is not the end of the file, continue
		while(!fin.eof())
		{
			//Retrieve each line of text
			getline(fin, str);
			//Display each line of text
			sout << str << "\n";
		}
	}

//Gets user file
std::string GetUserFile(std::string username)
{
	//Opens file
	std::ifstream fin(username);
	//Returns filename with extention
	return username + ".txt";
}
//Gets user inbox file
std::string GetInboxFile(std::string username)
{
	//Opens user inbox file
	std::ifstream fin(username + "-inbox" + ".txt");
	//Returns user inbox file with extention
	return username + "-inbox" + ".txt";
}
//Gets line of text from file
std::string GetLine(std::istream & sin)
{
	//Declare variable
	std::string next;
	//While retrieving line of text, continue
	while (std::getline(sin, next))
	{
		//If newline is detected, stop
		if ('\n')
			break;
	}
	//Returns line of text
	return next;
}
//Checks if user exists
bool DoesUserExist(std::string username)
{
	//Opens file
	std::ifstream fin(username + ".txt");
	//If file exists, returns true. If not, returns false
	return fin.good();
}
//Formats email layout
std::string FormatEmailString(std::string from, std::string to,
	std::string subject, std::string msg)
{
	//Returns sender, reciever, subject, and message on different lines
	return from + "\n" + to + "\n" + subject + "\n" + msg + "\n";
}