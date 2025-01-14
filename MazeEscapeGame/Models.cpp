#include "Constants.cpp";

struct Position {
	unsigned short rowIndex;
	unsigned short colIndex;
};

struct Map {
	int rowsCount;
	int colsCount;
	char** maze;
	Position playerPosition;
};

struct Game {
	bool keyFound = false;
	int coinsCollected = 0;
	int totalCoins = 0;
	int treasureFound = false;
	int level = 1;
	Map map;
};

struct Player {
	char name[USERNAME_MAX_LEN];
	int level = 1;
	int lives = DEFAULT_LIVES;
	int coins = 0;
	Game game;
};
