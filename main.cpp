#include <iostream>
#include <cstdlib>
#include <ctime>

constexpr int MAX_SIZE = 10;
constexpr int MIN_SIZE = 3;
constexpr int MAX_MINES = MAX_SIZE * 3;
constexpr int MIN_MINES = 1;

bool isValidInput(int mapSize, int mines)
{
	if (mapSize > MAX_SIZE or mapSize < MIN_SIZE or mines >	3 * mapSize or mines < MIN_MINES)
	{
		std::cout << "Invalid input. Try again" << std::endl;
		return false;
	}

	return true;
}

void validateInput(int& mapSize, int& mines)
{
	do
	{
		std::cout << "Enter size of the map (3 <= size <= 10):" << std::endl;
		std::cin >> mapSize;

		std::cout << "Enter number of hidden mines(1 <= mines <= " << 3 * mapSize <<"):" << std::endl;
		std::cin >> mines;
	} 
	while (!isValidInput(mapSize, mines));
}

void areValidCoordinates(int size, int x, int y)
{
	if (x >= size or x < 0 or y >= size or y < 0)
		std::cout << "Invalid coordinates" << std::endl;
}

void instructions()
{
	std::cout << "To play you have to type commands" << std::endl;
	std::cout << "Commands must be enterted like that: command x-coordinate y-coordinate" << std::endl;
	std::cout << "Possible commands are open/mark/unmark" << std::endl;
	std::cout << "Example: open 1 3" << std::endl;
	std::cout << "The coordinates of the top left corner are 0 0" << std::endl;
	std::cout << "To win you have to open all cells that are not mines" << std::endl;
	std::cout << "It is not necessary to mark all mine cells. Just don't open them" << std::endl;
	std::cout << "If you open a mine cell, you lose" << std::endl;
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

int randBetween(int min, int max) 
{
	return min + rand() % (max - min + 1); 
}

void uniqueCoordinates(int size, int mines, const int coordinates[][2], int& x, int& y, int current)
{
	bool isUnique = true; 
	do
	{
		x = randBetween(0, size - 1);
		y = randBetween(0, size - 1);
		
		for (int j = 0; j < current; j++)
		{
			if (x == coordinates[j][0] && y == coordinates[j][1])
			{
				isUnique = false;
				break;
			}
		}
	}
	while (!isUnique);
}

void generateUniqueRandomCoordinates(int size, int mines, int coordinates[][2])
{
	for (int i = 0; i < mines; i++)
	{
		int x = 0, y = 0;

		uniqueCoordinates(size, mines, coordinates, x, y, i);

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
			map[i][j] = '-';
		}
	}
}

void markMinesOnGrid(char map[][MAX_SIZE], int size, int numberOfMines, int mines[][2])
{
	createPlainMap(map, size);
	generateUniqueRandomCoordinates(size, numberOfMines, mines);

	for (int i = 0; i < numberOfMines; i++) 
	{
		int x = mines[i][0];
		int y = mines[i][1];
		map[x][y] = '#';
	}
}

int checkForAdjacentMines(const char map[][MAX_SIZE], int size, int row, int column)
{
	int counter = 0;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (map[row + i][column + j] == '#')
				counter++;
		}
	}

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

void drawMap(const char map[][MAX_SIZE], int size)
{
	std::cout << std::endl;
	for (int i = -2; i < size; i++)
	{
		if (i >= 0)
			std::cout << i << "| ";
		for (int j = 0; j < size; j++)
		{
			if (i == -2)
			{
				if (j == 0)
					std::cout << "   " << j;
				else
					std::cout << " " << j;
			}
			else if (i == -1)
			{
				if (j == 0)
					std::cout << "  --";
				else
					std::cout << "--";
			}
			else
				std::cout << map[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void revealCell(const char map[][MAX_SIZE], char maskMap[][MAX_SIZE], int size, int coordX, int coordY)
{
	if (coordX < 0 or coordX >= size or coordY < 0 or coordY >= size or maskMap[coordX][coordY] != '-')
		return;  

	maskMap[coordX][coordY] = map[coordX][coordY];

	if (checkForAdjacentMines(map, size, coordX, coordY))
		return;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			revealCell(map, maskMap, size, coordX + i, coordY + j);
		}
	}
}

void open(const char map[][MAX_SIZE], char maskMap[][MAX_SIZE],int size, int coordX, int coordY, bool& stillPlaying)
{
	if (maskMap[coordX][coordY] == '*')
	{
		std::cout << "Cannot open marked cell" << std::endl;
		return;
	}

	if (map[coordX][coordY] == '#')
	{
		stillPlaying = false;
		drawMap(map, size);
		std::cout << "Game over" << std::endl;
		return;
	}

	revealCell(map, maskMap, size, coordX, coordY);
}

void mark(char maskMap[][MAX_SIZE], int coordX, int coordY)
{
	if (maskMap[coordX][coordY] != '-')
	{
		std::cout << "Cannot mark revealed cell" << std::endl;
		return;
	}

	maskMap[coordX][coordY] = '*';
}

void unmark(char maskMap[][MAX_SIZE], int coordX, int coordY)
{
	if (maskMap[coordX][coordY] != '*')
		std::cout << "Cell is not marked" << std::endl;

	maskMap[coordX][coordY] = '-';
}

bool isGameWon(const char map[][MAX_SIZE], const char maskMap[][MAX_SIZE], int size)
{
	bool isWon = true;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if ((maskMap[i][j] == '-' or maskMap[i][j] == '*') && map[i][j] != '#')
				isWon = false;
		}
	}

	return isWon;
}

void play(char map[][MAX_SIZE], char maskMap[][MAX_SIZE], char* command, int size, int& coordX, int& coordY, bool& stillPlaying)
{
	drawMap(maskMap, size);

	std::cin >> command;
	std::cin >> coordX >> coordY;
	areValidCoordinates(size, coordX, coordY);

	if (areStringsEqual(command, "open"))
	{
		open(map, maskMap, size, coordX, coordY, stillPlaying);
		if (stillPlaying == false)
			return;
	}

	else if (areStringsEqual(command, "mark"))
		mark(maskMap, coordX, coordY);

	else if (areStringsEqual(command, "unmark"))
		unmark(maskMap, coordX, coordY);

	else
		std::cout << "Invalid command" << std::endl;

	stillPlaying = !isGameWon(map, maskMap, size);
}

void gameWon(const char map[][MAX_SIZE], const char maskMap[][MAX_SIZE], int mapSize)
{
	if (isGameWon(map, maskMap, mapSize))
	{
		drawMap(maskMap, mapSize);
		std::cout << "You won!" << std::endl;
	}
}

int main()
{
	srand(time(0));

	int mapSize = 0;
	int mines = 0;

	validateInput(mapSize, mines);

	char map[MAX_SIZE][MAX_SIZE];
	char maskMap[MAX_SIZE][MAX_SIZE];
	int mineCoordinates[MAX_SIZE][2];

	int x = 0, y = 0;
	char command[10];

	bool stillPlaying = true;

	instructions();
	generateMap(map, mapSize, mines, mineCoordinates);
	createPlainMap(maskMap, mapSize);

	do
	{	
		play(map, maskMap, command, mapSize, x, y, stillPlaying);
	} 
	while (stillPlaying);

	gameWon(map, maskMap, mapSize);

	return 0;
}