#include <iostream>
#include <fstream>
#include "Models.cpp"
using namespace std;


const char UPPER_FOLDER[] = "../test.txt";

const char FILE_NAME[] = "PractFiles.cpp";

const int BUFFER_SIZE = 1024;

const int USERNAME_MAX_LEN = 50;

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

// read the source code from your cpp and print in console
void printSourceCode() {
	std::ifstream in("dsadas.txt");

	if (!in.is_open()) {
		cout << "Error";
		return;
	}

	while (!in.eof()) {
		char buffer[BUFFER_SIZE];
		in.getline(buffer, BUFFER_SIZE);
		cout << buffer << endl;
	}

	in.close();
}

int fCreateUser(char* username) {

	if (username == nullptr) return -1;

	char dest[150] = "Users/";
	char* destination = customStrcat(dest, username);

	std::ofstream out(dest);

	if (!out.is_open()) return -1;

	out << "" << endl;
	out.close();
	return 0;
}


bool validateAccessInput(char ch) {
	return ch != 'r' && ch != 'l';
}

bool validateUsernameLen(char* username) {
	return my_StrLen(username) > USERNAME_MAX_LEN;
}
bool validateExistingUser(char* username) {
	char dest[150] = "Users/";
	std::ifstream in(customStrcat(dest, customStrcat(username, ".txt")));
	if (in.is_open()) {
		in.close();
		return true;
	}

	in.close();
	return false;
}

void printStartingScreen() {
	cout << "Welcome to the Maze Escape game" << endl;
	cout << "-------------------------------" << endl;
	cout << "  Type 'r' to Register." << endl;
	cout << "  Type 'l' to Login." << endl;
	cout << "-------------------------------" << endl;

}

void handleUserLogging() {

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

	if (inp == 'r')
	{
		cout << "Username: ";

		char username[120];
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
	}
	else if (inp == 'l')
	{
		cout << "Username: ";

		char username[121];
		cin >> username;

		while (validateUsernameLen(username))
		{
			cout << "Username too long!" << endl;
			cin >> username;
		}

		while (!validateExistingUser(username))
		{
			cout << "User doesn't exist" << endl;
			cin >> username;
		}

		player = loadUserData();
	}

}
int main()
{


	/*const char mapPath[] = "Maps/Level 1/Map 1.txt";
	const int rows = 10;
	const int cols = 15;

	char** pMatrix = readMap(mapPath, rows, cols);
	printMatrix(pMatrix, rows, cols);
	deleteMatrix(pMatrix, rows);
	return 0;*/


	handleUserLogging();

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