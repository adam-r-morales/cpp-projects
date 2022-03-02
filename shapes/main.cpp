#include "shapes.h"

int main()
{
	//Declare variables
	float x(0.0), y(0.0), z(0.0);
	char code;
	Shape* sptr;

	//Do-while to keep prgram rnning until user wants to quit
	do
	{
		//Define sptr as null to rest for each iteration of do-while
		sptr = nullptr;
		//Prompt user to choose shape or quit
		std::cout << "Enter code to calculate size of shape:(0 = Shape, 1 = Box, 2 = Cylinder, 3 = Rectangle)";
		std::cout << std::endl;
		std::cout << "or enter 'q' to quit" << std::endl;
		std::cin >> code;

		//Switch statement to allow user to input dimension of particular shape 
		switch (code)
		{
			case '0':
				std::cout << "Enter X,Y,Z:";
				std::cin >> x >> y >> z;

				std::cout << ' ' << x
					<< ' ' << y << ' '
					<< z;
				std::cout << '\n';

				sptr = new Shape(x,y,z);

				break;
			case '1':
				std::cout << "Enter L,W,H: ";
				std::cin >> x >> y >> z;

				std::cout << ' ' << x
					<< ' ' << y << ' '
					<< z;
				std::cout << '\n';

				sptr = new Box(x, y, z);

				break;
			case '2':
				std::cout << "Enter R,H:";
				std::cin >> x >> z;

				std::cout << ' ' << x
					<< ' ' << z;
				std::cout << '\n';

				sptr = new Cylinder(x, z);

				break;
			case '3':
				std::cout << "Enter L,W:";
				std::cin >> x >> y;

				std::cout << ' ' << x
					<< ' ' << y;
				std::cout << '\n';

				sptr = new Rectangle(x, y);

				break;
			case 'q':
				code = 'q';

				break;
			default:
				std::cerr << "ERROR: Incorrect code" << std::endl;
				break;

		}

		//If shape has been pick, then print out result
		//Delete data for next iteration
		if (sptr != nullptr)
		{
			std::cout << " Name: " << sptr->Name() << '\n';
			std::cout << " Volume: " << sptr->Volume() << '\n';
			std::cout << " Area:   " << sptr->Area() << '\n';
			delete sptr;
		}
	} while (code != 'q'); //End do-while

	return 0;
}