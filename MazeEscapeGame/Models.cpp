#include "Constants.cpp";

struct Position {
	short rowIndex;
	short colIndex;
};

struct Map {
	int rowsCount = 10;
	int colsCount = 15;
	char** maze;
	Position playerPosition;
};

struct Game {
	bool keyFound = false;
	int lives = DEFAULT_LIVES;
	int coinsCollected = 0;
	int totalCoins = 0;
	int treasureFound = false;
	Map map;
};

struct Player {
	char name[USERNAME_MAX_LEN];
	int currLevel = 1;
	int completedLevel = 1;
	int coins = 0;
	Game game;
};
