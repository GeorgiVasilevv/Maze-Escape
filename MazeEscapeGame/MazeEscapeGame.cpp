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

Player loadUserData(char* username) {
	Player pl = {};

	char dest[150] = "Users/";
	std::ifstream in(customStrcat(dest, username));
	if (!in.is_open())
	{
		return pl;
	}


	char buffer[BUFFER_SIZE];
	int value = in.get();
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
		cout << "User logged-in successfully!" << endl;
		cout << endl;
	}
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
			do
			{
				in.get(ch);
			} while (ch == '\n');

			map.maze[i][j] = ch;
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

int main()
{


	const char mapPath[] = "Maps/Level 1/Map 1.txt";
	const int rows = 10;
	const int cols = 15;
	const int level = 1;

	Game game = readMap(mapPath, rows, cols, level);
	printMap(game.map);
	deleteMap(game.map.maze, rows);
	return 0;


	/*Player user = handleUserLogging();*/

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