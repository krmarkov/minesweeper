#include <iostream>
#include <random>

constexpr int MAX_SIZE = 10;
constexpr int MAX_MINES = MAX_SIZE * 3;

bool isValidInput(int mapSize, int mines)
{
	if (mapSize > 10 or mapSize < 3 or mines > mapSize * 3 or mines < 1)
	{
		std::cout << "Invalid input. Try again" << std::endl;
		return false;
	}

	return true;
}

bool areStringsEqual(const char* first, const char* second) 
{
	if (!first or !second) 
		return false;

	while (*first != '\0' && *second != '\0')
	{
		if (*first != *second)
			return false;

		first++;
		second++;
	}
	return (*first == '\0' && *second == '\0');
}

int strLength(const char str[]) 
{
	int length = 0;
	int index = 0;

	while (str[index] != '\0') 
	{
		index++;
		length++;
	}
	return length;
}

void generateUniqueRandomCoordinates(int size, int mines, int coordinates[][2]) 
{
	// Seed for the random number generator
	std::random_device rd;

	// Mersenne Twister engine
	std::mt19937 gen(rd());

	// Uniform distribution in the range [0, size]
	std::uniform_int_distribution<int> distribution(0, size - 1);

	for (int i = 0; i < mines; i++) 
	{
		int x, y;

		// Generate and check for uniqueness in a single loop
		do 
		{
			x = distribution(gen);
			y = distribution(gen);

			// Check if the coordinates are unique
			bool isUnique = true;
			for (int j = 0; j < i; j++) 
			{
				if (x == coordinates[j][0] && y == coordinates[j][1]) 
				{
					isUnique = false;
					break;
				}
			}

			if (isUnique)
				break;  // Exit the loop if coordinates are unique

		} while (true);

		// Store the unique coordinates in the array
		coordinates[i][0] = x;
		coordinates[i][1] = y;
	}
}

void createPlainMap(char map[][MAX_SIZE], int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			map[i][j] = '0';
		}
	}
}

void markMinesOnGrid(char map[][MAX_SIZE], int size, int numberOfMines, int mines[][2])
{
	createPlainMap(map, size);
	generateUniqueRandomCoordinates(size, numberOfMines, mines);
	int mineCoordinateX = 0;
	int mineCoordinateY = 0;

	for (int i = 0; i < numberOfMines; i++) 
	{
		int x = mines[i][0];
		int y = mines[i][1];
		map[x][y] = '#';
	}
}

void checkIfMine(char c, int& counter)
{
	if (c == '#')
		counter++;
}

int checkForAdjacentMines(char map[][MAX_SIZE], int size, int row, int column)
{
	int counter = 0;

	checkIfMine(map[row + 1][column], counter);
	checkIfMine(map[row - 1][column], counter);
	checkIfMine(map[row][column + 1], counter);
	checkIfMine(map[row][column - 1], counter);
	checkIfMine(map[row + 1][column + 1], counter);
	checkIfMine(map[row + 1][column - 1], counter);
	checkIfMine(map[row - 1][column + 1], counter);
	checkIfMine(map[row - 1][column - 1], counter);

	return counter;
}

void numberOfAdjacentMines(char map[][MAX_SIZE], int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (map[i][j] != '#')
			{
				int numberOfAdjacentMines = checkForAdjacentMines(map, size, i, j);
				map[i][j] = (numberOfAdjacentMines + '0');
			}

		}
	}
}

void generateMap(char map[][MAX_SIZE], int size, int numberOfMines, int mines[][2])
{
	markMinesOnGrid(map, size, numberOfMines, mines);
	numberOfAdjacentMines(map, size);
}

void drawMap(char map[][MAX_SIZE], int size)
{
	std::cout << std::endl;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			std::cout << map[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void drawMaskMap(char map[][MAX_SIZE], int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			std::cout << '0' << " ";
		}
		std::cout << std::endl;
	}
}

void open(char map[][MAX_SIZE], int coordX, int coordY, bool& stillPlaying)
{
	if (map[coordX][coordY] == '#')
	{
		stillPlaying = false;
		std::cout << "Game over";
	}

	else
	{

	}
}

void mark(char map[][MAX_SIZE], int coordX, int coordY)
{
	map[coordX][coordY] = '*';
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

	char map[MAX_SIZE][MAX_SIZE];
	int mineCoordinates[MAX_SIZE][2];

	int x = 0, y = 0;
	char command[10];

	char openCommand[] = "open";
	char markCommand[] = "mark";

	bool stillPlaying = true;

	std::cout << "Command must be enterted like that: command x-coordinate y-coordinate" << std::endl;
	std::cout << "Possible commands are open/mark/unmark" << std::endl;
	std::cout << "Example: open 1 3" << std::endl;

	generateMap(map, mapSize, mines, mineCoordinates);
	do
	{
		drawMap(map, mapSize);
		std::cin >> command;
		std::cin >> x >> y;

		if (areStringsEqual(command, openCommand))
			open(map, x, y, stillPlaying);

		else if (areStringsEqual(command, markCommand))
			mark(map, x, y);
	} 
	while (stillPlaying);

	return 0;
}