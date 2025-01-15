#include <iostream>
#include <fstream>
#include "Models.cpp"


void deleteMap(char** matrix, int rows)
{
	if (matrix == nullptr) return;

	for (int i = 0; i < rows; i++)
	{
		delete[] matrix[i];
	}

	delete[] matrix;
}

void printMap(const Map map)
{
	if (map.maze == nullptr)
	{
		return;
	}

	for (int i = 0; i < map.rowsCount; i++)
	{
		for (int j = 0; j < map.colsCount; j++)
		{
			if (i == map.playerPosition.colIndex &&
				j == map.playerPosition.rowIndex)
			{
				std::cout << PLAYER_SYMBOL;
			}
			else {
				std::cout << map.maze[i][j];
			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}

char** initMap(int rowCount, int colCount)
{
	char** matrix = new char* [rowCount];

	for (int i = 0; i < rowCount; i++)
	{
		matrix[i] = new char[colCount];
	}

	return matrix;
}

//TODO Look into this
Game readMap(const char* mapPath, int rowCount, int colCount , int level)
{
	Game game = {};
	if (level > MAX_LEVEL)
	{
		return game;
	}
	
	if (mapPath == nullptr) return game;

	std::ifstream in(mapPath);
	if (!in.is_open())
	{
		return game;
	}


	game.level = level;
	Map map = {};
	map.rowsCount = rowCount;
	map.colsCount = colCount;

	char** maze = initMap(rowCount, colCount);
	map.maze = maze;

	int rowIdx = 0;

	while (rowIdx < rowCount
		&& in.getline(maze[rowIdx], colCount + 1))
	{
		rowIdx++;
	}

	in.close();

	return game;
}

void printGameInfo(Game game, Player player) {
	std::cout << "Level: " << game.level << std::endl;
	std::cout << "Lives: " << player.lives << std::endl;
	std::cout << "Coins: " << game.coinsCollected << std::endl;
	std::cout << "Key: ";

	if (game.keyFound)
	{
		std::cout << "Found" << std::endl;
	}
	else {
		std::cout << "Not found" << std::endl;
	}
}