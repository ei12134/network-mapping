#ifndef Algorithms_H_
#define Algorithms_H_

#include <iostream>
#include <float.h>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <vector>

#include "Exception.h"
#include "Intersection.h"
#include "Graph.h"

#define DEFAULT_VERTICES_FILE "vertices.csv"
#define DEFAULT_EDGES_FILE "edges.csv"

static const double PI = 3.141592653589793;

using namespace std;

inline bool is_Not_Number(const int & c);
bool is_All_Number(const string& s);

class Algorithms {
private:
	double radius;
	double area;
	Graph<Intersection> input;
	Graph<Intersection> result;
	void readVertexes(const char *filePath);
	void readEdges(const char *filePath);
public:
	Algorithms();
	Graph<Intersection> getInputGraph() const;
	Graph<Intersection> getResultGraph() const;
	void setGraph(Graph<Intersection> graph);
	void loadData(const char * vertexPath, const char * edgesPath);
	void saveData();
	vector<string> printVertices();
	vector<string> printEdges();
	void restoreResultGraph();
	void setArea(double area);
	void setRadius(double radius);
	double getArea() const;
	double getRadius() const;
};

#endif
