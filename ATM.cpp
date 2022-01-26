#include <iostream>
#include <iomanip>


void PrintAccountBalance(int & transaction, double account1, double account2);

int main()
{
	int transaction = 0;
	double checking = 100.50;
	double savings = 100.50;

	

	std::cout << "Welcome to People's Second Bank ATM" << "\n";
	PrintAccountBalance(transaction, savings, checking);

	int selection = 0;

	double amount = 0;
	while (selection != 7)
	{

		std::cout << "(1) to deposit to savings account" << std::endl;
		std::cout << "(2) to deposit to checking account" << std::endl;
		std::cout << "(3) to withdraw from savings sccount" << std::endl;
		std::cout << "(4) to withdraw from checking account" << std::endl;
		std::cout << "(5) to transfer from savings to checking account" << std::endl;
		std::cout << "(6) to transfer from checking to savings account" << std::endl;
		std::cout << "(7) to exit" << std::endl;

		std::cout << "What would you like to do?";
		std::cin >> selection;

		switch (selection)
		{
			
		case 1:
			std::cout << "How much would you like deposit in savings? $";
			std::cin >> amount;
			savings = savings + amount;
			break;

		case 2:
			std::cout << "How much would you like to deposit in checking? $";
			std::cin >> amount;
			checking = checking + amount;
			break;

		case 3:
			std::cout << "How much would you like to withdraw from savings? $";
			std::cin >> amount;
			if (amount <= savings)
				savings = savings - amount;
			else
				std::cout << "\nBalance exceeded!\n";
			break;

		case 4:
			std::cout << "How much would you like to withdraw from checking? $";
			std::cin >> amount;
			if (amount <= checking)
				checking = checking - amount;
			else
				std::cout << "\nBalance exceeded!\n";
			break;

		case 5:
			std::cout << "How much would you like to transfer from savings to checking? $";
			std::cin >> amount;
			if (amount <= savings)
				savings = savings - amount, checking = checking + amount;
			else
				std::cout << "\nBalance exceeded!\n";
			break;

		case 6:
			std::cout << "How much would you like to transfer from checking to savings? $";
			std::cin >> amount;
			if (amount <= checking)
				checking = checking - amount, savings = savings + amount;
			else 
				std::cout << "\nBalance exceeded!\n";
			break;

		case '\n':
		case '\t':
		case ' ':
			break;

		default:
			if (selection != 7)
				std::cout << "Invalid entry.\nTry again.\n";
			
			
		}
		PrintAccountBalance(transaction, savings, checking);


	}

	std::cout << "Press Enter to Continue...";
	std::cin.ignore();
	std::cin.get();
	return 0;

}


void PrintAccountBalance(int & transaction, double savings, double checking)
{
	transaction += 1;
	std::cout << "[" << transaction << "] Savings Balance: $";
	std::cout << std::setprecision(2) << std::fixed << savings << "\n";
	std::cout << "[" << transaction << "] Checking Balance: $";
	std::cout << std::setprecision(2) << std::fixed << checking << "\n";
	std::cout << std::endl;
}