#include <iostream>
#include "Cli.h"
#include "Algorithms.h"

using namespace std;
int main() {
//	Cli c;
//	c.menu();
	Algorithms a;
	a.loadData(DEFAULT_VERTEXES_FILE, DEFAULT_EDGES_FILE);
	a.print();
}
