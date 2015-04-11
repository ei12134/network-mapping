#ifndef Algorithms_H_
#define Algorithms_H_

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Exception.h"
#include "Intersection.h"
#include "Graph.h"

#define DEFAULT_VERTEXES_FILE "vertexes.csv"
#define DEFAULT_EDGES_FILE "edges.csv"

using namespace std;

class Algorithms {
private:
    Graph<Intersection> graph;
    void readVertexes(const char *filePath);
    void readEdges(const char *filePath);
public:
    Algorithms();

    Graph<Intersection> getGraph();

    void loadData(const char * vertexPath, const char * edgesPath);
    void saveData();
    vector<string> print();
};

#endif
