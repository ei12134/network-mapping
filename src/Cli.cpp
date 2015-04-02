#include "Cli.h"

Cli::Cli() {
	setConsole();
//
//	coloredString(true, "", "Hello World!", RED, true, BLACK, false, 1);
//	coloredString(true, "", "Hello World!", GREEN, true, BLACK, false, 1);
//	coloredString(true, "", "Hello World!", BLUE, true, BLACK, false, 1);
//	coloredString(true, "", "Hello World!", CYAN, true, BLACK, false, 1);
//	coloredString(true, "", "Hello World!", MAGENTA, true, BLACK, false, 1);
//	coloredString(true, "", "Hello World!", YELLOW, true, BLACK, false, 2);
//
//	coloredString(true, "", "Hello World!", GRAY, true, RED, true, 1);
//	coloredString(true, "", "Hello World!", GRAY, true, GREEN, true, 1);
//	coloredString(true, "", "Hello World!", GRAY, true, BLUE, true, 1);
//	coloredString(true, "", "Hello World!", GRAY, true, CYAN, true, 1);
//	coloredString(true, "", "Hello World!", GRAY, true, MAGENTA, true, 1);
//	coloredString(true, "", "Hello World!", GRAY, true, YELLOW, true, 2);
//
//	coloredString(true, "", "Hello World!", BLACK, true, RED, true, 1);
//	coloredString(true, "", "Hello World!", BLACK, true, GREEN, true, 1);
//	coloredString(true, "", "Hello World!", BLACK, true, BLUE, true, 1);
//	coloredString(true, "", "Hello World!", BLACK, true, CYAN, true, 1);
//	coloredString(true, "", "Hello World!", BLACK, true, MAGENTA, true, 1);
//	coloredString(true, "", "Hello World!", BLACK, true, YELLOW, true, 2);
//	while (!confirmOperation(false, "", "Exit?", RED, true, BLACK, true, 1))
//		;
}

void Cli::menu() {

}

void Cli::setColor(const int fgColor, bool fgIntensity, const int bgColor,
		bool bgIntensity) {
#if defined(_WIN32) || defined(_WIN64)
	SetConsoleTextAttribute(hConsoleOutput, fgColor | (fgIntensity << 3) | (bgColor << 4) | (bgIntensity << 7));
#else
//		cout << "\033[" << 0 << ";" << 40 + bgColor << "m";
	//	cout << "\033[" << 0 << ";" << 30 + fgColor << "m";
	cout << "\033[" << 0 << ";" << 40 + bgColor << ";" << 30 + fgColor << "m";
#endif
}

Cli::~Cli() {
	restoreConsole();
}

void Cli::clearScreen() {
#if defined(_WIN32) || defined(_WIN64)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount;
	COORD homeCoords = {0, 0};

	if ((HANDLE) hConsoleOutput == INVALID_HANDLE_VALUE)
	return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hConsoleOutput, &csbi))
	return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(hConsoleOutput, (TCHAR) ' ', cellCount,
					homeCoords, &count))
	return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(hConsoleOutput, csbi.wAttributes, cellCount,
					homeCoords, &count))
	return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hConsoleOutput, homeCoords);
#else
	system(CLEARSCREEN);
#endif
}

void Cli::setConsole() {
#if defined(_WIN32) || defined (_WIN64)
	// get handles
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);

	// backup current console configuration
	GetConsoleMode(hConsoleOutput, &fdwOldMode);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	GetConsoleScreenBufferInfo(hConsoleOutput, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;

	// set window title & size
	_COORD coord;
	coord.X = WIDTH;
	coord.Y = HEIGHT;
	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = HEIGHT - 1;
	Rect.Right = WIDTH - 1;
	SetConsoleWindowInfo(hConsoleOutput, TRUE, &Rect);
	SetConsoleScreenBufferSize(hConsoleOutput, coord);
	SetConsoleTitleA("Cli");
	setColor(GRAY,false,BLACK,false);
#else
	setColor(GREEN, true, BLACK, false);
	cout << string(30, '\n');
#endif
	clearScreen();
}

void Cli::restoreConsole() {
#if defined(_WIN32) || defined(_WIN64)
	// restore old console configuration
	SetConsoleMode(hConsoleInput, fdwOldMode);
	SetConsoleTextAttribute(hConsoleOutput, wOldColorAttrs);
#else
	cout << "\033[0;0;0m";
	cout << string(30, '\n');
#endif
	clearScreen();
}

char Cli::getKey() {
#ifdef _WIN32
	DWORD fdwSaveOldMode;
	GetConsoleMode(hConsoleInput, &fdwSaveOldMode);

// no need to read the return character nor mouse events
	DWORD mode = !ENABLE_ECHO_INPUT | !ENABLE_LINE_INPUT
	| !ENABLE_PROCESSED_INPUT | !ENABLE_MOUSE_INPUT;

	SetConsoleMode(hConsoleInput, mode);

	INPUT_RECORD lpBuffer;
	DWORD lpNumberOfEventsRead;
	DWORD specialKey;

	do {
		ReadConsoleInput(hConsoleInput, &lpBuffer, 1, &lpNumberOfEventsRead);
	}while (!lpBuffer.Event.KeyEvent.bKeyDown);

	specialKey = lpBuffer.Event.KeyEvent.wVirtualScanCode;
	char key = 0;

	if (specialKey == 72)
	key = ARROW_UP;
	else if (specialKey == 80)
	key = ARROW_DOWN;
	else if (specialKey == 83)
	key = DELETE_KEY;
	else if (specialKey == 1)
	key = ESCAPE_KEY;
	else
	key = lpBuffer.Event.KeyEvent.uChar.AsciiChar;

//FlushConsoleInputBuffer(hConsoleInput); // getline & special keys
// Restore input mode on exit.
	SetConsoleMode(hConsoleInput, fdwSaveOldMode);

	return key;

#elif _WIN64
	return getch();
#else
	static struct termios oldt, newt;

	/*tcgetattr gets the parameters of the current terminal
	 STDIN_FILENO will tell tcgetattr that it should write the settings
	 of stdin to oldt*/
	tcgetattr( STDIN_FILENO, &oldt);
	/*now the settings will be copied*/
	newt = oldt;

	/*ICANON normally takes care that one line at a time will be processed
	 that means it will return if it sees a "\n" or an EOF or an EOL*/
	newt.c_lflag &= ~(ICANON | ECHO);

	/*Those new settings will be set to STDIN
	 TCSANOW tells tcsetattr to change attributes immediately. */
	tcsetattr( STDIN_FILENO, TCSANOW, &newt);

	/*This is your part:
	 I choose 'e' to end input. Notice that EOF is also turned off
	 in the non-canonical mode*/
	char keys[32] = { 0 };
	fflush(stdout);
	read(STDIN_FILENO, keys, 4096);

	if (keys[0] == 27 && keys[1] == 91) {
		if (keys[2] == 51 && keys[3] == 126)
			keys[0] = DELETE_KEY;
		else if (keys[2] == 65)
			keys[0] = ARROW_UP;
		else if (keys[2] == 66)
			keys[0] = ARROW_DOWN;
		else if (keys[2] == 68)
			keys[0] = ARROW_LEFT;
		else if (keys[2] == 67)
			keys[0] = ARROW_RIGHT;
		else
			keys[0] = 0;
	} else if (keys[0] == 27 && keys[2] != 0)
		keys[0] = 0;

	/*restore the old settings*/
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	return keys[0];
#endif
}

void Cli::resetColor() {
#if defined(_WIN32) || defined(_WIN64)
	SetConsoleTextAttribute(hConsoleOutput,
			FOREGROUND_GREEN | FOREGROUND_INTENSITY | 0 | 0 | 0);
#else
	cout << "\033[0;40;32m";
#endif
}

void Cli::coloredString(bool centered, const string &spacing, const string &s,
		const int fgColor, bool fgIntensity, const int bgColor,
		bool bgIntensity, const int newLines) {
	if (centered) {
		int center = (int) ((WIDTH - s.size()) / 2);
		cout << string(center, ' ');
	} else {
		cout << spacing;
	}
	setColor(fgColor, fgIntensity, bgColor, bgIntensity);
	cout << s;
	resetColor();
	cout << string(newLines, '\n');
}

bool Cli::confirmOperation(bool centered, const string &spacing,
		const string &s, const int fgColor, bool fgIntensity, const int bgColor,
		bool bgIntensity, const int newLines) {
	if (centered) {
		int center = (int) ((WIDTH - s.size()) / 2);
		cout << string(center, ' ');
	} else {
		cout << spacing;
	}
	setColor(fgColor, fgIntensity, bgColor, bgIntensity);
	cout << s << " [y/n]";
	char answer = getKey();
	resetColor();

	if (tolower(answer) == 'y')
		return true;
	else
		return false;
}
