#include <iostream>
#include <fstream>
#include "Models.cpp"
using namespace std;

const char UPPER_FOLDER[] = "../test.txt";

const char FILE_NAME[] = "PractFiles.cpp";

const int BUFFER_SIZE = 1024;

void clearConsole() {
	cout << "\033[;H"; // Moves cursor to the top left
	cout << "\033[J"; // Clears the console
}

// Remove all error flags and characters from the input buffer
void clearInputBuffer() {
	// because of using both getline and cin we have to cin.ignore;
	// cin leaves the newline character in the stream which will be read as input from the getline

	cin.clear();	// clears errors flags from the cin
	cin.sync();		// discard unread characters from the input buffer
	cin.ignore(1000, '\n');	// discard characters from the input buffer
}

int my_StrLen(char* str) {
	if (str == nullptr) return -1;

	int destLen = 0;
	while (str[destLen] != '\0')
	{
		destLen++;
	}
	return destLen;
}

int my_atoi(char* str) {
	int res = 0;
	int strLen = my_StrLen(str);

	for (int i = 0; i < strLen; i++)
	{
		res = res * 10 + str[i] - '0';
	}

	return res;
}

char* customStrcat(char* destination, const char* source) {
	if (destination == nullptr || source == nullptr)
	{
		return nullptr;
	}

	int destLen = my_StrLen(destination);

	int sourceLen = 0;
	while (source[sourceLen] != '\0')
	{
		destination[destLen] = source[sourceLen];
		sourceLen++;
		destLen++;
	}
	destination[destLen] = '\0';

	return destination;
}

bool validateAccessInput(char ch) {
	return ch != 'r' && ch != 'l';
}

bool validateUsernameLen(char* username) {
	return my_StrLen(username) > USERNAME_MAX_LEN;
}

bool validateExistingUser(char* username) {
	char dest[150] = "Users/"; // TODO fix this
	std::ifstream in(customStrcat(dest, username)); // TODO fix this
	if (in.is_open()) {
		in.close();
		return true;
	}

	in.close();
	return false;
}

bool validateCoordinate(const Position& position, int rows, int cols)
{
	return position.rowIndex < rows && position.colIndex < cols;
}

Player loadUserData(char* username) {
	Player pl = {};

	char dest[150] = "Users/";
	std::ifstream in(customStrcat(dest, username));
	if (!in.is_open())
	{
		return pl;
	}


	char buffer[BUFFER_SIZE];
	in.getline(buffer, BUFFER_SIZE);
	pl.level = my_atoi(buffer);

	in.getline(buffer, BUFFER_SIZE);
	pl.lives = my_atoi(buffer);

	in.getline(buffer, BUFFER_SIZE);
	pl.coins = my_atoi(buffer);

	in.getline(buffer, BUFFER_SIZE);
	pl.game.keyFound = my_atoi(buffer);

	in.getline(buffer, BUFFER_SIZE);
	pl.game.coinsCollected = my_atoi(buffer);

	in.getline(buffer, BUFFER_SIZE);
	pl.game.totalCoins = my_atoi(buffer);

	in.getline(buffer, BUFFER_SIZE);
	pl.game.treasureFound = my_atoi(buffer);

	in.close();
	return pl;
}

int fCreateUser(char* username) {

	if (username == nullptr) return -1;

	char dest[150] = "Users/"; // TODO fix this
	char* destination = customStrcat(dest, username); // TODO fix this

	std::ofstream out(dest);

	if (!out.is_open()) return -1;

	Player pl = {};
	out << pl.level << endl;
	out << pl.lives << endl;
	out << pl.coins << endl;
	out << pl.game.keyFound << endl;
	out << pl.game.coinsCollected << endl;
	out << pl.game.totalCoins << endl;
	out << pl.game.treasureFound << endl;
	out.close();
	return 0;
}

void printStartingScreen() {
	cout << "Welcome to the Maze Escape game" << endl;
	cout << "-------------------------------" << endl;
	cout << "  Type 'r' to Register." << endl;
	cout << "  Type 'l' to Login." << endl;
	cout << "-------------------------------" << endl;
}

Player handleUserLogging() {

	clearConsole();
	printStartingScreen();

	char inp;
	cin >> inp;
	while (validateAccessInput(inp))
	{
		clearConsole();
		printStartingScreen();

		cout << "Invalid input! Try again." << endl;

		clearInputBuffer();
		cin >> inp;
	}

	Player player = {};
	while (inp == 'r')
	{
		cout << "Username: ";

		char username[120]; // TODO fix this
		cin >> username;

		while (validateUsernameLen(username))
		{
			cout << "Username too long!" << endl;
			cin >> username;
		}
		while (validateExistingUser(username))
		{
			cout << "User already exists!" << endl;
			cin >> username;
		}

		fCreateUser(username);

		cout << "User created successfully!" << endl;
		cout << endl;
		printStartingScreen();
		cin >> inp;
	}
	if (inp == 'l')
	{
		cout << "Username: ";

		char username[121]; // TODO fix this
		cin >> username;

		while (validateUsernameLen(username))
		{
			cout << "Username too long!" << endl;
			cin >> username;
		}

		while (!validateExistingUser(username)) //TODO there is a problem when user tries to log in but there is no users in the database
		{
			cout << "User doesn't exist" << endl;
			cin >> username;
		}

		player = loadUserData(username);

	}
	clearConsole();
	return player;
}

void deleteMap(char** matrix, int rows)
{
	if (matrix == nullptr) return;

	for (int i = 0; i < rows; i++)
	{
		delete[] matrix[i];
	}

	delete[] matrix;
}

void printMap(const Map& map)
{
	if (map.maze == nullptr)
	{
		return;
	}

	for (int i = 0; i < map.rowsCount; i++)
	{
		for (int j = 0; j < map.colsCount; j++)
		{
			if (i == map.playerPosition.rowIndex &&
				j == map.playerPosition.colIndex)
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

Game readMap(const char* mapPath, int rowCount, int colCount, int level)
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


	for (short i = 0; i < rowCount; i++)
	{
		for (short j = 0; j < colCount; j++)
		{
			char ch;
			in.get(ch);

			if (ch == '\n')
			{
				in.get(ch);
			}

			if (ch == PLAYER_SYMBOL)
			{
				map.playerPosition.rowIndex = i;
				map.playerPosition.colIndex = j;

				map.maze[i][j] = SPACE_SYMBOL;
			}
			else {
				map.maze[i][j] = ch;
			}
			if (ch == COIN_SYMBOL)
			{
				game.totalCoins++;
			}
		}
	}

	game.map = map;
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

void printGameRules() {
	std::cout << "Use the keys to move:" << std::endl;
	std::cout << " W - UP" << std::endl;
	std::cout << " A - LEFT" << std::endl;
	std::cout << " S - DOWN" << std::endl;
	std::cout << " D - RIGHT" << std::endl;
}

char toLower(char ch) {
	if (ch >= 'A' && ch <= 'Z') {
		return ch + 32;
	}
	return ch;
}

bool movePlayer(Position& position, char command) {
	command = toLower(command);
	if (command == 'w')
	{
		position.rowIndex--;
	}
	else if (command == 'a') {
		position.colIndex--;
	}
	else if (command == 's') {
		position.rowIndex++;
	}
	else if (command == 'd') {
		position.colIndex++;
	}
	else {
		return false;
	}
	return true;
}
Position findNextPortal(const Map& map, const Position& currPortal)
{
	Position nextPortal = {};

	if (map.maze == nullptr)
	{
		return nextPortal;
	}

	int startRow, startCol;

	if (currPortal.rowIndex == map.rowsCount - 1
		&& currPortal.colIndex == map.colsCount - 1)
	{
		startRow = 0;
		startCol = 0;
	}
	else
	{
		startRow = currPortal.rowIndex;
		startCol = currPortal.colIndex + 1;
	}

	bool portalFound = false;
	for (int i = startRow; i < map.rowsCount; i++)
	{
		if (i != startRow)
		{
			startCol = 0;
		}

		for (int j = startCol; j < map.colsCount; j++)
		{
			if (map.maze[i][j] == PORTAL_SYMBOL)
			{
				portalFound = true;
				nextPortal.rowIndex = i;
				nextPortal.colIndex = j;
				break;
			}

			if (i == map.rowsCount - 1 && j == map.colsCount - 1)
			{
				i = 0;
				j = -1;
			}
		}

		if (portalFound)
		{
			break;
		}
	}

	return nextPortal;
}

void updateMaze(Player& player, Game& game, char command) {
	char** maze = game.map.maze;
	Position& pos = game.map.playerPosition;
	Position newPos = pos;

	if (maze == nullptr)
	{
		return;
	}
	if (!movePlayer(newPos, command))
	{
		return;
	}
	if (!validateCoordinate(newPos, game.map.rowsCount, game.map.colsCount))
	{
		return;
	}


	switch (maze[newPos.rowIndex][newPos.colIndex])
	{
	case WALL_SYMBOL:
		player.lives--;
		break;

	case SPACE_SYMBOL:
		pos = newPos;
		break;

	case COIN_SYMBOL:
		game.coinsCollected++;
		pos = newPos;
		maze[newPos.rowIndex][pos.colIndex] = SPACE_SYMBOL;
		break;

	case KEY_SYMBOL:
		game.keyFound = true;
		pos = newPos;
		maze[newPos.rowIndex][pos.colIndex] = SPACE_SYMBOL;
		break;

	case PORTAL_SYMBOL:
		pos = findNextPortal(game.map, newPos);
		break;

	case TREASURE_SYMBOL:
		pos = newPos;

		if (game.keyFound)
		{
			game.treasureFound = true;
		}
		break;
	}
}


int main()
{
	const char mapPath[] = "Maps/Level 1/Map 1.txt";
	const int rows = 10;
	const int cols = 15;
	const int level = 1;

	Player pl = handleUserLogging();

	cout << "User logged-in successfully!" << endl;
	cout << endl;

	Game game = readMap(mapPath, rows, cols, level);
	printMap(game.map);

	printGameRules();
	char inp;
	cin >> inp;

	while (inp != 'E')
	{
		updateMaze(pl, game, inp);
		clearConsole();
		printMap(game.map);

		cin >> inp;
	}

	deleteMap(game.map.maze, rows);


	return 0;

}

/**
 * Solution to course project # 3
 * Introduction to programming course
 * Faculty of Mathematics and Informatics of Sofia University
 * Winter semester 2023/2024
 *
 * @author Georgi Vasilev
 * @idnumber 0MI0600499
 * @compiler VC
 *
 * <code for maze escape game>
 */