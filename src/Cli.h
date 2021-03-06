#ifndef CLI_H_
#define CLI_H_

#include <cstdio>
#include <cstdlib>
#include <float.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "Algorithms.h"
#include "graphviewer.h"
#include "Graph.h"
using namespace std;

#ifdef _WIN32

#define CLI_BLACK 0
#define CLI_BLUE 1
#define CLI_GREEN 2
#define CLI_CYAN CLI_BLUE | CLI_GREEN
#define CLI_RED 4
#define CLI_YELLOW CLI_GREEN | CLI_RED
#define CLI_MAGENTA CLI_BLUE | CLI_RED
#define CLI_GRAY CLI_BLUE | CLI_GREEN | CLI_RED

#define BACKSPACE_KEY 8
#define RETURN_KEY 13
#define CLEARSCREEN "cls"

#include <windows.h>
#include <wincon.h>
#define PROMPT_SYMBOL "> "
static const char hSeparator = 196;
static const char progressBar = 254;

#else // Other OS's
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

#define CLI_BLACK 0
#define CLI_RED 1
#define CLI_GREEN 2
#define CLI_YELLOW CLI_GREEN | CLI_RED
#define CLI_BLUE 4
#define CLI_MAGENTA CLI_BLUE | CLI_RED
#define CLI_CYAN CLI_BLUE | CLI_GREEN
#define CLI_GRAY CLI_BLUE | CLI_GREEN | CLI_RED

#define BACKSPACE_KEY 127
#define RETURN_KEY 10
#define CLEARSCREEN "clear"

#define PROMPT_SYMBOL "$ "
static const string hSeparator = "\u2501";
static const string progressBar = "\u25A0";

#endif

#define HEIGHT 25
#define WIDTH 80

#define HALF_TAB string(4,' ')
#define TAB "\t"
#define TWO_TABS TAB TAB
#define THREE_TABS TWO_TABS TAB
#define THREE_HALF_TABS TWO_TABS TAB + HALF_TAB
#define FOUR_TABS TWO_TABS TWO_TABS
#define FIVE_TABS FOUR_TABS TAB

#define TAB_KEY 9
#define ESCAPE_KEY 27
#define ARROW_DOWN 28
#define ARROW_UP 29
#define ARROW_LEFT 30
#define ARROW_RIGHT 31
#define SPACE_BAR 32
#define DELETE_KEY 83

#define MAX_LINES 12

class Cli {
private:
#if defined(_WIN32) || defined (_WIN64)
	HANDLE hConsoleOutput;
	HANDLE hConsoleInput;
	DWORD fdwOldMode;
	DWORD wOldColorAttrs;
#endif
	Algorithms a;
	bool infBgI, infFgI, strBgI, strFgI, errBgI, errFgI;
	int infBg, infFg, errBg, errFg, strBg, strFg;
	
	GraphViewer *gv;
public:
	Cli();
	Cli(int newStrFg, bool newStrFgI, int newStrBg, bool newStrBgI,
			int newInfFg, bool newInfFgI, int newInfBg, bool newInfBgI,
			int newErrFg, bool newErrFgI, int newErrBg, bool newErrBgI);
	~Cli();

	void setConsole();
	void restoreConsole();
	void setTheme(int newStrFg, bool newStrFgI, int newStrBg, bool newStrBgI,
			int newInfFg, bool newInfFgI, int newInfBg, bool newInfBgI,
			int newErrFg, bool newErrFgI, int newErrBg, bool newErrBgI);
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
	inline void textMsg(const string &spacing, const string &s,
			const int newLines);
	inline void infoMsg(const string &s, const int newLines);
	inline void errorMsg(const string &s, const int newLines);
	void cmdMsg(const string &spacing, const size_t index, const string &s,
			const int newLines);
	void displayHeader(string& header);

	template<class T>
	string repeatStr(const T& s, const size_t n) {
#if defined(_WIN32) || defined (_WIN64)
		return string(n, s);
#else
		string r = s;
		for (size_t i = 0; i < n; i++)
			r += s;
		return r;
#endif
	}

	void menu();
	void graphInfo(const vector<Vertex<Intersection> *> vertexSet);
	
	bool repeatedEdge(vector<Edge<Intersection> > edges, Edge<Intersection> e);
	void graphViewer(const vector<Vertex<Intersection> *> vertexSet);

	int displayContainer(vector<string> vec, string listName, string labels,
			string sortStr);
	bool readArea(double& area);
	bool readCentrals(unsigned int& centrals);
	void mstMenu();
};

#endif // CLI_H_
