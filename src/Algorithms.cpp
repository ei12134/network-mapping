#include "Algorithms.h"

Algorithms::Algorithms() {
}

void Algorithms::loadData(const char * vertexFileName,const char * edgesFileName) {
	input.clear();
	readVertexes(vertexFileName);
	readEdges(edgesFileName);
}

void Algorithms::readVertexes(const char *filePath) {
	fstream file;
	string line, sId, sType;
	stringstream ss, cnv;
	int x, y;
	int id;
	int type;
	
	file.open(filePath);
	if (file.is_open()) {
		while (file.good()) {
			try {
				// clear used variables
				line.clear();
				ss.clear();
				sId.clear();
				sType.clear();
				
				// read a line
				getline(file, line, '\n');
				ss << line;
				
				// parse a line
				if (!getline(ss, sId, ';'))
					throw Exception<string>("Error reading id", "Vertex");
				cnv << sId;
				cnv >> id;
				cnv.str(std::string());
				cnv.clear();
				
				if (!getline(ss, sId, ';'))
					throw Exception<string>("Error reading x", "Vertex");
				cnv << sId;
				cnv >> x;
				cnv.str(std::string());
				cnv.clear();
				
				if (!getline(ss, sId, ';'))
					throw Exception<string>("Error reading y", "Vertex");
				cnv << sId;
				cnv >> y;
				cnv.str(std::string());
				cnv.clear();
				
				if (!getline(ss, sType))
					throw Exception<string>("Error reading type", "Vertex");
				cnv << sType;
				cnv >> type;
				cnv.str(std::string());
				cnv.clear();
				
				// add intersection to the graph
				Intersection i(id, x, y, type);
				input.addVertex(i);
				// add copy to result graph
				result.addVertex(i);
			} catch (Exception<string> &e) {
			}
		}
	}
	file.close();
}

void Algorithms::readEdges(const char *filePath) {
	fstream file;
	string line, sSrcId, sDstId, sDist;
	stringstream ss, cnv;
	double distance;
	unsigned int srcId, dstId;
	
	file.open(filePath);
	if (file.is_open()) {
		while (file.good()) {
			try {
				// clear used variables
				line.clear();
				ss.clear();
				sSrcId.clear();
				sDstId.clear();
				sDist.clear();
				
				// read a line
				getline(file, line, '\n');
				ss << line;
				
				// parse a line
				if (!getline(ss, sSrcId, ';'))
					throw Exception<string>("Error reading source id", "Edges");
				cnv << sSrcId;
				cnv >> srcId;
				cnv.clear();
				
				if (!getline(ss, sDstId, ';'))
					throw Exception<string>("Error reading destiny id",
											"Edges");
					cnv << sDstId;
				cnv >> dstId;
				cnv.clear();
				
				if (!getline(ss, sDist))
					throw Exception<string>("Error reading distance", "Edges");
				cnv << sDist;
				cnv >> distance;
				cnv.clear();
				
				// find vertexes
				Intersection src(srcId, 0, 0, INTERSECTION);
				Intersection dst(dstId, 0, 0, INTERSECTION);
				
				// add edge to the graph
				input.addEdge(src, dst, distance);
				input.addEdge(dst, src, distance);
				
				// add copy to result graph
				result.addEdge(src, dst, distance);
				result.addEdge(dst, src, distance);
			} catch (Exception<string> &e) {
			}
		}
	}
	file.close();
}

Graph<Intersection> Algorithms::getInputGraph() const {
	return input;
}

Graph<Intersection> Algorithms::getResultGraph() const {
	return result;
}

void Algorithms::setGraph(Graph<Intersection> graph) {
	this->input = graph;
}

vector<string> Algorithms::printVertices() {
	vector<string> p = input.print(false);
	return p;
}

vector<string> Algorithms::printEdges() {
	vector<string> p = input.print(true);
	return p;
}

void Algorithms::setArea(double area) {
	this->area = area;
	this->radius = sqrt(abs(area) / PI);
}

void Algorithms::setRadius(double radius) {
	this->radius = radius;
	this->area = radius * radius * PI;
}

double Algorithms::getArea() const {
	return area;
}

double Algorithms::getRadius() const {
	return radius;
}

inline bool is_Not_Number(const int & c) {
	return !(c >= 48 && c <= 57);
}

bool is_All_Number(const string& s) {
	return find_if(s.begin(), s.end(), is_Not_Number) == s.end();
}
