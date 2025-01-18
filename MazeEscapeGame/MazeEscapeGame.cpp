#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Models.cpp"

const int BUFFER_SIZE = 1024;

const int rows = 10;
const int cols = 15;

void clearConsole() {
	std::cout << "\033[;H"; // Moves cursor to the top left
	std::cout << "\033[J"; // Clears the console
}

// Remove all error flags and characters from the input buffer
void clearInputBuffer() {
	// because of using both getline and cin we have to cin.ignore;
	// cin leaves the newline character in the stream which will be read as input from the getline

	std::cin.clear();	// clears errors flags from the cin
	std::cin.sync();		// discard unread characters from the input buffer
	std::cin.ignore(1000, '\n');	// discard characters from the input buffer
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

char* my_StrCpy(char* destination, const char* source) {

	int length = 0;
	while (source[length] != '\0')
	{
		destination[length] = source[length];
		length++;
	}

	destination[length] = '\0';
	return destination;
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

char* my_StrCat(char* destination, const char* source) {
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

char toLower(char ch) {
	if (ch >= 'A' && ch <= 'Z') {
		return ch + 32;
	}
	return ch;
}

void swap(int& first, int& second)
{
	int temp = first;
	first = second;
	second = temp;
}

void initRandom()
{
	srand(time(0));
}

int getRandomNumber(int min, int max)
{
	if (min > max)
	{
		swap(min, max);
	}
	initRandom();
	int random = min + rand() % (max - min + 1);
	return random;
}

bool validateAccessInput(char ch) {
	return ch != 'r' && ch != 'l';
}

bool validateLevelCommandInput(char ch) {
	return ch != 'n' && ch != 'c';
}

bool validateUsernameLen(char* username) {
	return my_StrLen(username) > USERNAME_MAX_LEN;
}

bool validateExistingUser(char* username) {
	char dest[150] = "Users/"; // TODO fix this
	std::ifstream in(my_StrCat(dest, username));
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

void printMazeToFile(char** maze, int rows, int cols, std::ofstream& out) {
	if (maze == nullptr)
	{
		return;
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{

			out << maze[i][j];
		}
		out << std::endl;
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

void getMapPath(int level, char* dest) {
	if (dest == nullptr) {
		return;
	}

	my_StrCat(dest, "Maps/Level ");

	char levelChar[2];
	levelChar[0] = level + '0';
	levelChar[1] = '\0';
	my_StrCat(dest, levelChar);

	my_StrCat(dest, "/Map ");

	int loadMapNumber = getRandomNumber(1, 2);
	char mapChar[2];
	mapChar[0] = loadMapNumber + '0';
	mapChar[1] = '\0';
	my_StrCat(dest, mapChar);

	my_StrCat(dest, ".txt");
}

Player loadUserData(char* username) {
	Player pl = {};
	my_StrCat(pl.name, username);

	char dest[150] = "Users/";
	std::ifstream in(my_StrCat(dest, username));
	if (!in.is_open())
	{
		return pl;
	}

	Game userGame = readMap(dest, pl.game.map.rowsCount, pl.game.map.colsCount, pl.currLevel); // I really just need this to read the map so i improvised.
	pl.game = userGame;


	char buffer[BUFFER_SIZE];
	for (int i = 0; i < pl.game.map.rowsCount; i++)
	{
		in.getline(buffer, BUFFER_SIZE);
	}
	in.getline(buffer, BUFFER_SIZE);
	pl.currLevel = my_atoi(buffer);

	in.getline(buffer, BUFFER_SIZE);
	pl.completedLevel = my_atoi(buffer);

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

	in.getline(buffer, BUFFER_SIZE);
	pl.game.lives = my_atoi(buffer);

	in.getline(buffer, BUFFER_SIZE);
	pl.game.map.playerPosition.rowIndex = my_atoi(buffer);

	in.getline(buffer, BUFFER_SIZE);
	pl.game.map.playerPosition.colIndex = my_atoi(buffer);

	in.close();

	return pl;
}

int fCreateUser(char* username) {

	if (username == nullptr) return -1;

	char dest[150] = "Users/"; // TODO fix this
	char* destination = my_StrCat(dest, username); // TODO fix this

	std::ofstream out(dest);

	if (!out.is_open()) return -1;

	Player pl = {};

	char destPath[30] = {}; // TODO fix this size
	getMapPath(pl.currLevel, destPath);
	Game game = readMap(destPath, rows, cols, pl.currLevel);
	printMazeToFile(game.map.maze, pl.game.map.rowsCount, pl.game.map.colsCount, out);

	pl.game = game;
	out << pl.currLevel << std::endl;
	out << pl.completedLevel << std::endl;
	out << pl.coins << std::endl;
	out << pl.game.keyFound << std::endl;
	out << pl.game.coinsCollected << std::endl;
	out << pl.game.totalCoins << std::endl;
	out << pl.game.treasureFound << std::endl;
	out << pl.game.lives << std::endl;
	out << pl.game.map.playerPosition.rowIndex << std::endl;
	out << pl.game.map.playerPosition.colIndex << std::endl;

	out.close();
	return 0;
}

void printStartingScreen() {
	std::cout << "Welcome to the Maze Escape game" << std::endl;
	std::cout << "-------------------------------" << std::endl;
	std::cout << "  Type 'r' to Register." << std::endl;
	std::cout << "  Type 'l' to Login." << std::endl;
	std::cout << "-------------------------------" << std::endl;
}

void handleUserRegistration(char username[]) {
	while (validateExistingUser(username)) {
		std::cout << "User already exists!" << std::endl;
		std::cout << "Username: ";
		std::cin >> username;
	}

	fCreateUser(username);
	std::cout << "User created successfully!" << std::endl;
	std::cout << std::endl;
}

Player handleUserLogin(char username[]) {
	while (!validateExistingUser(username)) {
		std::cout << "User doesn't exist." << std::endl;
		std::cout << "Username: ";
		std::cin >> username;
	}

	return loadUserData(username);
}

Player handleUserLogging() {
	clearConsole();
	printStartingScreen();

	char inp;
	std::cin >> inp;
	inp = toLower(inp);

	while (validateAccessInput(inp)) {
		clearConsole();
		printStartingScreen();
		std::cout << "Invalid input! Try again." << std::endl;
		clearInputBuffer();
		std::cin >> inp;
		inp = toLower(inp);
	}

	Player player = {};

	while (inp == 'l' || inp == 'r') {
		char username[USERNAME_MAX_LEN];

		std::cout << "Username: ";
		std::cin >> username;

		while (validateUsernameLen(username)) {
			std::cout << "Username too long!" << std::endl;
			std::cout << "Username: ";
			std::cin >> username;
		}

		if (inp == 'r') {
			handleUserRegistration(username);
			printStartingScreen();
			std::cin >> inp;
			inp = toLower(inp);
		}
		else if (inp == 'l') {
			player = handleUserLogin(username);
			break;
		}
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

void printGameInfo(Game game, Player player) {
	std::cout << "Level: " << player.currLevel << std::endl;
	std::cout << "Lives: " << game.lives << std::endl;
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
	std::cout << std::endl;
	std::cout << "Use the keys to move:" << std::endl;
	std::cout << " W - UP" << std::endl;
	std::cout << " A - LEFT" << std::endl;
	std::cout << " S - DOWN" << std::endl;
	std::cout << " D - RIGHT" << std::endl;
	std::cout << " ---------" << std::endl;
	std::cout << " Press [Q] to Quit" << std::endl;
}

void printLevelSelection() {
	std::cout << "Please pick one of the following:" << std::endl;
	std::cout << "--------------------------------" << std::endl;
	std::cout << "[C] Continue last played level." << std::endl;
	std::cout << "[N] Play a level again." << std::endl;
	std::cout << std::endl;
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
		game.lives--;
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

bool handleLevelPicking(Player& player) {

	printLevelSelection();

	char cmd;
	std::cin >> cmd;
	cmd = toLower(cmd);

	while (validateLevelCommandInput(cmd))
	{
		clearConsole();
		printLevelSelection();

		std::cout << "Invalid input! Try again." << std::endl;

		clearInputBuffer();
		std::cin >> cmd;
		cmd = toLower(cmd);
	}
	if (cmd == 'c')
	{
		return false;
	}

	std::cout << "Pick a level you have completed. (1 - " << player.completedLevel << ")" << std::endl;
	int num;
	std::cin >> num;

	while (num <1 || num > player.completedLevel)
	{
		std::cout << "Invalid level picked. Try again." << std::endl;
		clearInputBuffer();
		int num;
		std::cin >> num;
	}

	player.currLevel = num;
	return true;
}

void saveProgress(const Player& pl) {
	char dest[150] = "Users/";
	std::ofstream out(my_StrCat(dest, pl.name));

	if (!out.is_open()) {
		std::cout << "Error: Unable to save user data!" << std::endl;
		return;
	}

	printMazeToFile(pl.game.map.maze, pl.game.map.rowsCount, pl.game.map.colsCount, out);

	out << pl.currLevel << std::endl;
	out << pl.completedLevel << std::endl;
	out << pl.coins << std::endl;
	out << pl.game.keyFound << std::endl;
	out << pl.game.coinsCollected << std::endl;
	out << pl.game.totalCoins << std::endl;
	out << pl.game.treasureFound << std::endl;
	out << pl.game.lives << std::endl;
	out << pl.game.map.playerPosition.rowIndex << std::endl;
	out << pl.game.map.playerPosition.colIndex << std::endl;

	out.close();
}



int main()
{
	Player pl = handleUserLogging();

	while (true)
	{

		bool wantsToPlayLevelAgain = handleLevelPicking(pl);
		if (wantsToPlayLevelAgain)
		{
			char destPath[30] = {}; // TODO fix this size
			getMapPath(pl.currLevel, destPath);
			Game game = readMap(destPath, rows, cols, pl.currLevel);
			pl.game = game;
		}

		char inp;
		bool hasQuit = true;
		bool hasWon = false;
		while (true)
		{
			clearConsole();
			printGameInfo(pl.game, pl);
			printMap(pl.game.map);

			if (pl.game.treasureFound)
			{
				pl.coins += pl.game.coinsCollected;
				if (pl.currLevel != MAX_LEVEL && pl.currLevel == pl.completedLevel)
				{
					pl.completedLevel++;
					pl.currLevel++;
				}
				std::cout << "Congratulations you completed the level!" << std::endl;
				hasQuit = false;
				hasWon = true;
				break;
			}
			if (pl.game.lives == 0)
			{
				std::cout << "You died! Better luck next time!" << std::endl;
				hasQuit = false;
				break;
			}

			printGameRules();
			std::cin >> inp;
			if (toLower(inp) == QUIT_SYMBOL)
			{
				break;
			}

			updateMaze(pl, pl.game, inp);
		}

		if (pl.completedLevel == MAX_LEVEL && hasWon)
		{
			std::cout << "Congratulations you won the game!" << std::endl;
			break;
		}

		if (!hasQuit)
		{
			char destPath[30] = {}; // TODO fix this size
			getMapPath(pl.currLevel, destPath);
			Game game = readMap(destPath, rows, cols, pl.currLevel);
			pl.game = game;
		}

		saveProgress(pl);

	}
	deleteMap(pl.game.map.maze, rows);

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