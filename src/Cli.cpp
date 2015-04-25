#include "Cli.h"

Cli::Cli() {
	// normal text colour
	strFg = CLI_GREEN;
	strFgI = true;
	strBg = CLI_BLACK;
	strBgI = false;

	// information colour
	infFg = CLI_BLACK;
	infFgI = false;
	infBg = CLI_GRAY;
	infBgI = true;

	// error messages colour
	errFg = CLI_GRAY;
	errFgI = true;
	errBg = CLI_RED;
	errBgI = false;

	setConsole();
}

Cli::Cli(int newStrFg, bool newStrFgI, int newStrBg, bool newStrBgI,
		int newInfFg, bool newInfFgI, int newInfBg, bool newInfBgI,
		int newErrFg, bool newErrFgI, int newErrBg, bool newErrBgI) {
	// normal text colour
	strFg = newStrFg;
	strFgI = newStrFgI;
	strBg = newStrBg;
	strBgI = newStrBgI;

	// information colour
	infFg = newInfFg;
	infFgI = newInfFgI;
	infBg = newInfBg;
	infBgI = newInfBgI;

	// error messages colour
	errFg = newErrFg;
	errFgI = newErrFgI;
	errBg = newErrBg;
	errBgI = newErrBgI;

	setConsole();
}

Cli::~Cli() {
	restoreConsole();
}

void Cli::setColor(const int fgColor, bool fgIntensity, const int bgColor,
		bool bgIntensity) {
#if defined(_WIN32) || defined(_WIN64)
	SetConsoleTextAttribute(hConsoleOutput, fgColor | (fgIntensity << 3) | (bgColor << 4) | (bgIntensity << 7));
#else
	//	cout << "\e[" << fgIntensity << ";" << 30 + fgColor << "m";
	//	cout << "\e[" << bgIntensity << ";" << 40 + bgColor + (60 * bgIntensity)<< "m";

	cout << "\033[" << fgIntensity << ";" << 40 + bgColor + (60 * bgIntensity)
			<< ";" << 30 + fgColor << "m";
#endif
}

void Cli::setTheme(int newStrFg, bool newStrFgI, int newStrBg, bool newStrBgI,
		int newInfFg, bool newInfFgI, int newInfBg, bool newInfBgI,
		int newErrFg, bool newErrFgI, int newErrBg, bool newErrBgI) {
	// normal text colour
	strFg = newStrFg;
	strFgI = newStrFgI;
	strBg = newStrBg;
	strBgI = newStrBgI;

	// information colour
	infFg = newInfFg;
	infFgI = newInfFgI;
	infBg = newInfBg;
	infBgI = newInfBgI;

	// error messages colour
	errFg = newErrFg;
	errFgI = newErrFgI;
	errBg = newErrBg;
	errBgI = newErrBgI;
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
	if (!FillConsoleOutputCharacter(hConsoleOutput, (TCHAR) ' ', cellCount, homeCoords, &count))
	return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(hConsoleOutput, csbi.wAttributes, cellCount, homeCoords, &count))
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
	SetConsoleTitleA("Telefones");
	setColor(CLI_GRAY,false,CLI_BLACK,false);
#else
	setColor(strFg, strFgI, strBg, strBgI);
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
	| !ENABLE_PROCESSED_INPUT | !ENABLE_MOUSE_INPUT | !ENABLE_WINDOW_INPUT;

	SetConsoleMode(hConsoleInput, mode);

	INPUT_RECORD lpBuffer;
	DWORD lpNumberOfEventsRead;
	DWORD specialKey;

	do {
		ReadConsoleInput(hConsoleInput, &lpBuffer, 1, &lpNumberOfEventsRead);
	} while (!(lpBuffer.EventType == KEY_EVENT) || !(lpBuffer.Event.KeyEvent.bKeyDown));

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
	cout << "\033[" << strFgI << ";" << 40 + strBg + (60 * strBgI) << ";"
			<< 30 + strFg << "m";
//	cout << "\033[0;40;32m";
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

inline void Cli::textMsg(const string &spacing, const string &s,
		const int newLines) {
	coloredString(false, spacing, s, strFg, strFgI, strBg, strBgI, newLines);
}

inline void Cli::infoMsg(const string &s, const int newLines) {
	coloredString(true, "", s, infFg, infFgI, infBg, infBgI, newLines);
}

inline void Cli::errorMsg(const string &s, const int newLines) {
	coloredString(true, "", s, errFg, errFgI, errBg, errBgI, newLines);
}

void Cli::cmdMsg(const string &spacing, const size_t index, const string &s,
		const int newLines) {
	stringstream ss;
	void textMsg(const string &spacing, const string &s, const int newLines);
	ss << "[";
	ss << index;
	ss << "] ";
	coloredString(false, spacing, ss.str(), infBg, infBgI, strBg, strBgI, 0);
	coloredString(false, "", s, strFg, strFgI, strBg, strBgI, newLines);
	resetColor();
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

void Cli::displayHeader(string& header) {
	unsigned int size = header.size();
	unsigned int dynSizeLeft = ceil((WIDTH - size) / 2);
	unsigned int dynSizeRight = dynSizeLeft;
	char lineSeparator;

#if defined (_WIN32) || defined (_WIN64)
	lineSeparator = '\r';
#else
	lineSeparator = '\n';
#endif
	if (dynSizeLeft + dynSizeRight + size < WIDTH)
		dynSizeRight++;

	coloredString(false, "", string(WIDTH, ' '), strBg, strBgI, strFg, strFgI,
			0);
	cout << lineSeparator;

	coloredString(false, "", string(dynSizeLeft, ' '), strBg, strBgI, strFg,
			strFgI, 0);
	coloredString(false, "", header, strBg, strBgI, strFg, strFgI, 0);
	coloredString(false, "", string(dynSizeRight, ' '), strBg, strBgI, strFg,
			strFgI, 0);

	coloredString(false, "", string(WIDTH, ' '), strBg, strBgI, strFg, strFgI,
			0);
	cout << lineSeparator << "\n\n";
}

void Cli::menu() {
	char input;
	bool exit = false;
	const size_t menuCmdsSize = 7;
	string menuCmds[menuCmdsSize] = { "Build graph", "MST",
			"GraphViewer", "Info", "Vertices", "Edges\n", "Quit" };
	string spacing = string((WIDTH - menuCmds[0].size() - 4) / 2, ' ');
	string exitMsg = "Quit?";
	string headerMsg = "Telefones";
	string infMsg, errMsg;
	Graph<Intersection> graph, result;

	do {
		clearScreen();
		displayHeader(headerMsg);
		stringstream ss;
		graph = a.getInputGraph();
		result = a.getResultGraph();

		for (size_t i = 0; i < menuCmdsSize; i++)
			cmdMsg(spacing, (i + 1), menuCmds[i], 1);
		cout << "\n";
		
		// Print pending messages
		if (infMsg.size() > 0) {
			infoMsg(infMsg, 2);
			infMsg.clear();
		}
		if (errMsg.size() > 0) {
			errorMsg(errMsg, 2);
			errMsg.clear();
		}

		coloredString(false, spacing, PROMPT_SYMBOL, strFg, strFgI, strBg,
				strBgI, 0);

		input = getKey();
		switch (input) {
		case '1':
			a.loadData(DEFAULT_VERTICES_FILE, DEFAULT_EDGES_FILE);
			graph = a.getInputGraph();
			ss << " " << graph.getNumVertex() << " vertexes and " << graph.getNumEdges() << " edges loaded ";
			infMsg = ss.str();
			break;
		case '2':;
			if (graph.getVertexSet().size() == 0)
				errMsg = " Empty or invalid graph ";
			else {
				mstMenu();
			}
			break;
		case '3':
			if (graph.getVertexSet().size() != 0) {
				graphInfo(graph.getVertexSet(), false, true);
			} else
				errMsg = " Empty or invalid graph ";
			break;
		case '4':
			if (graph.getVertexSet().size() != 0)
				graphInfo(graph.getVertexSet(), false, false);
			else
				errMsg = " Empty or invalid graph ";
			break;
		case '5':
			displayContainer(a.printVertices(), "Graph vertices",
					"\t\tId\t\tX\t\tY\t\tType", "");
			break;
		case '6':
			displayContainer(a.printEdges(), "Graph edges",
					"\tSource\t\t\tDestiny\t\t\tDistance", "");
			break;
		case '7':
			if (confirmOperation(false, "", exitMsg, CLI_RED, true, CLI_BLACK,
					false, 0))
				exit = true;
			break;
		case ESCAPE_KEY:
			if (confirmOperation(false, "", exitMsg, CLI_RED, true, CLI_BLACK,
					false, 0))
				exit = true;
			break;
		default:
			break;
		}
	} while (!exit);
}

int Cli::displayContainer(vector<string> vec, string listName, string labels,
		string sortStr) {
	unsigned int vecSize = vec.size(), pCount = 1, vLimit = 0, i = 0, progress;
	float pLimit;
	bool done = false;
	char key;
	string vLimitMsg;
	string sortedMsg;
	if (sortStr.size() > 0) {
		sortedMsg = "Sorted by " + sortStr
				+ (sortStr.size() > 5 ? +TAB : +TWO_TABS);
		vLimitMsg =
				" [ESC] to interrupt [s] to sort or any other key to continue...";
	} else
		vLimitMsg = " [ESC] to interrupt or any other key to continue...";
	if (vecSize == 0)
		pLimit = 1;
	else
		pLimit = ceil(static_cast<float>(vecSize) / MAX_LINES);
	while (1) {
		do {
			vLimit = 0;
			progress = ceil((13.0 / pLimit) * pCount);
			clearScreen();
			displayHeader(listName);
			ostringstream progrStr;
			progrStr << sortedMsg << "Page " << pCount << " of " << pLimit

			<< " [" << repeatStr(progressBar, progress)
					<< string((13 - progress), ' ') << "]";
			if (sortStr.size() == 0) {
				cout << THREE_TABS << progrStr.str();
			} else {
				cout << TWO_TABS << progrStr.str();
			}
			cout << endl << endl;
			cout << " " << repeatStr(hSeparator, 77) << " " << endl;
			cout << " " << labels << endl;
			cout << " " << repeatStr(hSeparator, 77) << " " << endl;
			if (vecSize == 0) {
				string nothing = " Nothing to show here :( ";
				cout << string(5, '\n');
				errorMsg(nothing, 0);
				cout << string(6, '\n');
			}
			while (vLimit < MAX_LINES && i < vecSize && !done) {
				setColor(CLI_GRAY, true, CLI_BLACK, false);
				cout << " " << vec[i];
				resetColor();
				cout << endl;
				i++;
				vLimit++;
				if (vLimit == MAX_LINES && i < vecSize) {
					pCount++;
					cout << " " << repeatStr(hSeparator, 77) << endl
							<< vLimitMsg;
					key = getKey();
					if (key == ESCAPE_KEY)
						done = true;
					else if (tolower(key) == 's' && sortStr.size() > 0) {
						return -1;
					}
				}
			}
			if (vecSize != 0)
				cout << string((MAX_LINES - vLimit), '\n');
		} while (i < vecSize && !done);
		if (done)
			break;
		else if (i == vecSize) {
			cout << " " << repeatStr(hSeparator, 77) << endl << vLimitMsg;
			key = getKey();
			if (tolower(key) == 's' && sortStr.size() > 0) {
				return -1;
			} else
				break;
		}
	}
	return 0;
}

void Cli::graphInfo(const vector<Vertex<Intersection> *> vertexSet, bool path, bool gui) {
	string headerMsg = "Graph information";

	// Get statistical values from the graph
	int totalVertices = vertexSet.size();
	int totalEdges = 0;
	double totalDistance = 0.0;;

	for (size_t x = 0; x < vertexSet.size(); x++) {
		vector<Edge<Intersection> > adj = vertexSet[x]->getAdj();
		totalEdges += adj.size();
		for (size_t i = 0; i < adj.size(); i++)
			totalDistance += adj[i].getDistance();
	}
	// Non directed graph
	totalDistance /= 2;

	// Display data
	clearScreen();
	displayHeader(headerMsg);

	string spacing = THREE_TABS + HALF_TAB;

	coloredString(false, spacing, "Number of vertices: ", infBg, infBgI, infFg,
			infFgI, 0);
	cout << totalVertices << "\n";
	coloredString(false, spacing, "Number of edges: ", infBg, infBgI, infFg,
			infFgI, 0);
	cout << totalEdges << "\n";
	coloredString(false, spacing, "Total distance: ", infBg, infBgI, infFg,
			infFgI, 0);
	cout << totalDistance << " m " << "\n\n\n";
	
	if (gui) {
		coloredString(true, "", "    Central   ", CLI_BLACK, false, CLI_RED, true, 1);
		coloredString(true, "", "     House    ", CLI_GRAY, true, CLI_BLUE, true, 1);
		coloredString(true, "", " Intersection ", CLI_BLACK, false, CLI_YELLOW, true, 2);
		infoMsg(" Lauching graphical viewer ", 3);
		coloredString(true, "", "Press any key to continue...", strFg,
					  strFgI, strBg, strBgI, 0);
		cout.flush();
		graphViewer(vertexSet, path);
	} else {
		coloredString(true, "", "Press any key to continue...", strFg,
					  strFgI, strBg, strBgI, 0);
	}

	getKey();
}

void Cli::graphViewer(const vector<Vertex<Intersection> *> vertexSet, bool path) {
	stringstream ss;
	string label;
	
	// Create windows and define colours
	gv = new GraphViewer(800, 600, false);
	gv->createWindow(800, 600);
	gv->defineVertexColor("yellow");
	gv->defineEdgeColor("black");
	
	// Add nodes
	for (size_t i = 0; i < vertexSet.size(); i++) {
		Intersection temp = vertexSet[i]->getInfo();
		// Add node
		gv->addNode(temp.getId(), temp.getX(), temp.getY());
		if (temp.getType() == CENTRAL)
			gv->setVertexColor(temp.getId(), "red");
		else if (temp.getType() == HOUSE)
			gv->setVertexColor(temp.getId(), "blue");
		
		// Add node label
		ss << temp.getId();
		gv->setVertexLabel(temp.getId(), ss.str());
		
		// Clear & reset stringstream
		ss.str(std::string());
		ss.clear();
	}
	
	if (!path){
	  // Add edges
	  int counter = 0;
	  for (size_t x = 0; x < vertexSet.size(); x++) {
		
		vector<Edge<Intersection> > adj = vertexSet[x]->getAdj();
		for (size_t i = 0; i < adj.size(); i++) {
		  // Add edge
		  gv->addEdge(counter, vertexSet[x]->getInfo().getId(),
					  adj[i].getDest()->getInfo().getId(), EdgeType::DIRECTED);
		  
		  // Add label
		  
		  ss << adj[i].getDistance();
		  label = ss.str() + " m";
		  gv->setEdgeLabel(counter, label);
		  
		  counter++;
		  
		  // Clear & reset stringstream
		  ss.str(std::string());
		  ss.clear();
		}
	  }
	}
	
	else{
		int counter = 0;
		for (size_t x = 0; x < vertexSet.size(); x++) {
			if (vertexSet[x]->path != NULL) {
				
				gv->addEdge(counter, vertexSet[x]->getInfo().getId(),
							vertexSet[x]->path->getInfo().getId(),
							EdgeType::UNDIRECTED);
				//Add label
				ss << vertexSet[x]->getDist();
				label = ss.str() + " m";
				gv->setEdgeLabel(counter, label);
				counter++;
				
			}
			// Clear & reset stringstream
			ss.str(std::string());
			ss.clear();
		}
	}
	
	// Redraw
	gv->rearrange();
}

bool Cli::readCentrals(int& centrals) {
	string input;
	getline(cin, input);
	if (input.size() > 0 && is_All_Number(input) && input[0] != '0') {
		stringstream ss;
		ss << input;
		ss >> centrals;
		return true;
	}
	return false;
}

bool Cli::readArea(double& area) {
	string input;
	getline(cin, input);
	if (input.size() > 0 && is_All_Number(input)) {
		stringstream ss;
		ss << input;
		ss >> area;
		return true;
	}
	return false;
}

void Cli::mstMenu() {
	char input;
	bool exit = false;
	string header = "Minimum spanning tree";
	int centralCount = 1;
	double area = DBL_MAX; // meters squared
	string infMsg, errMsg;
	string spacing = FOUR_TABS;
	
	const size_t cmdsSize = 4;
	string cmds[cmdsSize] = { "Centrals: ", "Area: ",
		"Compute MST", "Exit" };
		do {
			clearScreen();
			displayHeader(header);

			// reset graphs & distances
			Graph<Intersection> result = a.getResultGraph();
			Graph<Intersection> bestResult = a.getResultGraph();

			double distance = DBL_MAX;
			double tmpDistance = DBL_MAX;

			// convert values
			string centralCountStr;
			stringstream ss;
			ss << centralCount;
			centralCountStr = ss.str();
			
			string areaStr;
			if (centralCount > 1 || area == DBL_MAX)
				areaStr = "N/D";
			else {
				stringstream ss2;
				ss2 << area;
				areaStr = ss2.str();
			}
			
			// Display commands
			coloredString(false, spacing, "[1] ", infBg, infBgI, infFg, infFgI, 0);
			textMsg("", cmds[0], 0);
			coloredString(false, "", centralCountStr, infBg, infBgI, infFg, infFgI, 1);
			
			coloredString(false, spacing, "[2] ", infBg, infBgI, infFg, infFgI, 0);
			textMsg("", cmds[1], 0);
			coloredString(false, "", areaStr, infBg, infBgI, infFg, infFgI, 1);
			
			cmdMsg(spacing, (2 + 1), cmds[2], 2);

			for (size_t i = 3; i < cmdsSize; i++)
				cmdMsg(spacing, (i + 1), cmds[i], 1);
			
			// Print pending messages
			if (infMsg.size() > 0) {
				infoMsg(infMsg, 2);
				infMsg.clear();
			}
			if (errMsg.size() > 0) {
				errorMsg(errMsg, 2);
				errMsg.clear();
			}
			cout << endl << spacing << PROMPT_SYMBOL;
			
			input = getKey();
			switch (input) {
				case '1':
					cout << "Quantity: ";
					if (!readCentrals(centralCount))
						errMsg = " Enter a valid central amount number ";
					else if (centralCount > 1)
						area = DBL_MAX;
					break;
				case '2':
					cout << "Area (m^2): ";
					if (!readArea(area))
						errMsg = " Enter a valid area value ";
					else 
						centralCount = 1;
					break;
				case '3':
					if (result.getVertexSet().size() == 0)
						errMsg = " Empty or invalid graph ";
					else {
						for (size_t i = 0; i < result.getVertexSet().size(); i++) {
							// restore graph
							a.restoreResultGraph();
							result = a.getResultGraph();
							tmpDistance = result.bfMst(i,area);

							if (tmpDistance < distance) {
								distance = tmpDistance;
								bestResult = result;
							}
						}
						// display minimum spanning tree
						graphInfo(bestResult.getVertexSet(), true, true);
					}
					
					break;
				case '4':
					exit = true;
					break;
				case ESCAPE_KEY:
					exit = true;
					break;
				default:
					break;
			}
		} while (!exit);
}