/*
 * Algorithms.h
 */

#ifndef Algorithms_H_
#define Algorithms_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Graph.h"
#include "Intersection.h"
#include "Exception.h"

using namespace std;
class Algorithms {
private:
	Graph<Intersection> graph;

	bool readVertex(const char *filePath);
	bool readEdges(const char *filePath);

public:
	Algorithms(){};

	void loadData(const char * vertexPath,const char * edgesPath);
	void saveData();
};

#endif /* Algorithms_H_ */
