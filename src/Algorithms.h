#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#include <algorithm>
#include <float.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

#include "Exception.h"
#include "Intersection.h"
#include "Graph.h"

#define DEFAULT_EDGES_FILE "edges.csv"
#define DEFAULT_VERTICES_FILE "vertices.csv"

using namespace std;

inline bool is_Not_Number(const int & c);
bool is_All_Number(const string& s);

class Algorithms {
private:
	Graph<Intersection> input;
	Graph<Intersection> result;
	void readVertexes(const char *filePath);
	void readEdges(const char *filePath);
public:
	Algorithms();
	Graph<Intersection> getInputGraph() const;
	void setGraph(Graph<Intersection> graph);
	void loadData(const char * vertexPath, const char * edgesPath);
	void saveData();
	vector<string> printVertices();
	vector<string> printEdges();
	void restoreResultGraph();
};

#endif // ALGORITHMS_H_
