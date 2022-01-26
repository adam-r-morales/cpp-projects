#include "Header.h"

int main()
{
	float x(0.0), y(0.0), z(0.0);
	char code;
	Shape* sptr;

	do
	{
		sptr = nullptr;
		std::cout << "Enter code to calculate size of shape:(0 = Shape, 1 = Box, 2 = Cylinder, 3 = Rectangle)";
		std::cout << std::endl;
		std::cout << "or enter 'q' to quit" << std::endl;

		std::cin >> code;

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

		}

		if (sptr != nullptr)
		{
			std::cout << " Name: " << sptr->Name() << '\n';
			std::cout << " Volume: " << sptr->Volume() << '\n';
			std::cout << " Area:   " << sptr->Area() << '\n';
			delete sptr;
		}
	} while (code != 'q');

	return 0;
}