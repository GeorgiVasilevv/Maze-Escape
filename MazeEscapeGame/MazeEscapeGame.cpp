#include <iostream>
#include <fstream>
using namespace std;

const char UPPER_FOLDER[] = "../test.txt";

const char FILE_NAME[] = "PractFiles.cpp";

const int BUFFER_SIZE = 1024;

// read the source code from your cpp and print in console
void printSourceCode() {
	std::ifstream in(FILE_NAME);

	if (!in.is_open()) {
		cout << "Error";
		return;
	}

	while (!in.eof()) {
		char buffer[BUFFER_SIZE];
		in.getline(buffer, BUFFER_SIZE);
		cout << buffer;
	}

	in.close();
}

void readCharArrayWithSpaces() {
	std::ofstream out(FILE_NAME);

	char test[] = "abc de";
	out << test;

	out.close();

	std::ifstream in(FILE_NAME);

	char test2[BUFFER_SIZE];
	in.getline(test2, BUFFER_SIZE);

	in.close();
}

int fCreateUser(char* username) {
	std::ofstream out(username);

	if (!out.is_open() || username == nullptr) return -1;

	out << "dsadas" << endl;
	out.close();
	return 0;
}

// Clear console
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

//void levelOne() {
//	const int ROWS = 10;
//	const int COLS = 15;
//
//	char maze[ROWS][COLS] = {
//	{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
//	{ '#', ' ', ' ', ' ', '@', ' ', 'C', ' ', '%', ' ', 'C', ' ', ' ', 'C', '#' },
//	{ '#', '#', '#', '#', ' ', 'C', ' ', '#', ' ', 'C', ' ', '#', '#', ' ', '#' },
//	{ '#', ' ', ' ', ' ', '#', '#', '#', ' ', 'C', ' ', '#', ' ', ' ', ' ', '#' },
//	{ '#', ' ', '%', ' ', ' ', '#', ' ', ' ', 'C', ' ', '%', ' ', 'C', ' ', '#' },
//	{ '#', 'C', ' ', 'C', ' ', 'C', ' ', 'C', ' ', '#', ' ', '#', ' ', ' ', '#' },
//	{ '#', 'C', ' ', 'C', ' ', '#', '#', '#', ' ', ' ', '#', 'C', ' ', 'C', '#' },
//	{ '#', 'C', ' ', ' ', 'C', ' ', 'C', ' ', '&', ' ', ' ', 'C', ' ', 'C', '#' },
//	{ '#', 'C', ' ', '%', ' ', ' ', ' ', 'X', ' ', 'C', ' ', '%', ' ', ' ', '#' },
//	{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };
//
//	for (int i = 0; i < 10; i++) {
//		for (int j = 0; j < 15; j++) {
//			cout << maze[i][j] << " ";
//		}
//		cout << endl;
//	}
//}

bool validateAccessInput(char ch) {
	return ch != 'r' && ch != 'l';
}

void printStartingScreen() {
	cout << "Welcome to the Maze Escape game" << endl;
	cout << "-------------------------------" << endl;
	cout << "  Type 'r' to Register." << endl;
	cout << "  Type 'l' to Login." << endl;
	cout << "-------------------------------" << endl;

}

int main()
{

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

	if (inp == 'r')
	{
		char username[20];
		cin >> username;
		fCreateUser(username);
	}

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