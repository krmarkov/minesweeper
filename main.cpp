/**
*
* Solution to course project #5
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Kristian Markov
* @idnumber 7MI0600348
* @compiler VC
*
* <main file>
*
*/

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

bool areValidCoordinates(int size, int x, int y)
{
	if (x >= size or x < 0 or y >= size or y < 0)
		return false;

	return true;
}

void instructions()
{
	std::cout << "To play you have to type commands" << std::endl;
	std::cout << "Commands must be enterted like that: command x-coordinate y-coordinate" << std::endl;
	std::cout << "Possible commands are open/mark/unmark" << std::endl;
	std::cout << "Example: open 1 3" << std::endl;
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

int randBetween(int min, int max, int size) 
{
	return min + rand() % size; 
}

void generateUniqueRandomCoordinates(char map[][MAX_SIZE], int size)
{
	int x = randBetween(0, size - 1, size);
	int y = randBetween(0, size - 1, size);
	if (map[x][y] != '#')
		map[x][y] = '#';
	else
		generateUniqueRandomCoordinates(map, size);
}

void markMinesOnMap(char map[][MAX_SIZE], int size, int numberOfMines)
{
	createPlainMap(map, size);

	for (int i = 0; i < numberOfMines; i++) 
	{
		generateUniqueRandomCoordinates(map, size);
	}
}

int checkForAdjacentMines(const char map[][MAX_SIZE], int size, int row, int column)
{
	int counter = 0;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (row + i >= size or row + i < 0 or column + j >= size or column + j < 0)
				continue;
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

void generateMap(char map[][MAX_SIZE], int size, int numberOfMines)
{
	markMinesOnMap(map, size, numberOfMines);
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

	if (maskMap[coordX][coordY] != '-' && areValidCoordinates(size, coordX, coordY))
	{
		std::cout << "This cell is already opened" << std::endl;
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
	//drawMap(map, size);
	drawMap(maskMap, size);

	std::cin >> command;
	std::cin >> coordX >> coordY;
	if (!areValidCoordinates(size, coordX, coordY))
		std::cout << "Invalid coordinates" << std::endl;

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

	int x = 0, y = 0;
	char command[10];

	bool stillPlaying = true;

	instructions();
	generateMap(map, mapSize, mines);
	createPlainMap(maskMap, mapSize);

	do
	{	
		play(map, maskMap, command, mapSize, x, y, stillPlaying);
	} 
	while (stillPlaying);

	gameWon(map, maskMap, mapSize);

	return 0;
}