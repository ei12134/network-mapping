#ifndef CLI_H_
#define CLI_H_

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

#ifdef _WIN32

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN BLUE | GREEN
#define RED 4
#define YELLOW GREEN | RED
#define MAGENTA BLUE | RED
#define GRAY BLUE | GREEN | RED

#define BACKSPACE_KEY 8
#define RETURN_KEY 13
#define CLEARSCREEN "cls"

#include <windows.h>
#include <wincon.h>
#define PROMPT_SYMBOL "> "

#else // Other OS's
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW GREEN | RED
#define BLUE 4
#define MAGENTA BLUE | RED
#define CYAN BLUE | GREEN
#define GRAY BLUE | GREEN | RED

#define BACKSPACE_KEY 127
#define RETURN_KEY 10
#define CLEARSCREEN "clear"

#define PROMPT_SYMBOL "$ "

#endif

#define HEIGHT 25
#define WIDTH 80

#define HALF_TAB string(4,' ')
#define TAB "\t"

#define TAB_KEY 9
#define ESCAPE_KEY 27
#define ARROW_DOWN 28
#define ARROW_UP 29
#define ARROW_LEFT 30
#define ARROW_RIGHT 31
#define SPACE_BAR 32
#define DELETE_KEY 83

class Cli {
private:
#if defined(_WIN32) || defined (_WIN64)
	HANDLE hConsoleOutput;
	HANDLE hConsoleInput;
	DWORD fdwOldMode;
	DWORD wOldColorAttrs;
#endif
public:
	Cli();
	~Cli();
	void menu();
	void setConsole();
	void restoreConsole();
	void clearScreen();
	void coloredString(bool centered, const string &spacing, const string &s,
			const int fgColor, bool fgIntensity, const int bgColor,
			bool bgIntensity, const int newLines);
	bool confirmOperation(bool centered, const string &spacing, const string &s,
			const int fgColor, bool fgIntensity, const int bgColor,
			bool bgIntensity, const int newLines);
	char getKey();
	void setColor(const int fgColor, bool fgIntensity, const int bgColor,
			bool bgIntensity);
	void resetColor();
};

#endif /* CLI_H_ */
