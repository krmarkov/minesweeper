#include <iostream>
constexpr int MAX_SIZE = 10;

bool isValidInput(int mapSize, int mines)
{
	if (mapSize > 10 or mapSize < 3 or mines > mapSize * 3 or mines < 1)
	{
		std::cout << "Try again" << std::endl;
		return false;
	}

	return true;
}
int main()
{
	int mapSize = 0;
	int mines = 0;

	do
	{
		std::cout << "Enter size of the map (3 <= size <= 10):" << std::endl;
		std::cin >> mapSize;
		
		std::cout << "Enter number of hidden mines(1 <= mines <= 3 * size):" << std::endl;
		std::cin >> mines;
	} 
	while (!isValidInput(mapSize, mines));


	int x = 0, y = 0;
	char command[10];
	std::cout << "Command must be enterted like that: command x-coordinate y-coordinate" << std::endl;
	std::cout << "Possible commands are open/mark/unmark" << std::endl;
	std::cout << "Example: open 1 3" << std::endl;

	while (true)
	{
		std::cin.getline(command, 10);
		std::cin >> x >> y;
		
	}
}