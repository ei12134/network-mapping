#include "Algorithms.h"

Algorithms::Algorithms() {
}

void Algorithms::loadData(const char * vertexFileName,const char * edgesFileName) {
	
	// read input graph
	input.clear();
	readVertexes(vertexFileName);
	readEdges(edgesFileName);
	
	// create a disposable copy
	restoreResultGraph();
}

void Algorithms::readVertexes(const char *filePath) {
	fstream file;
	string line, sId, sX, sY, sType;
	stringstream ss, cnv;
	int x, y;
	int id;
	int type;
	unsigned int lineCounter = 0;
	
	file.open(filePath);
	if (file.is_open()) {
		while (file.good()) {
			lineCounter++;
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
				if (!getline(ss, sId, ';') || !is_All_Number(sId))
					throw Exception<string>("Error reading id", "Vertex");
				cnv << sId;
				cnv >> id;
				cnv.str(std::string());
				cnv.clear();
				
				if (!getline(ss, sX, ';') || !is_All_Number(sX))
					throw Exception<string>("Error reading x", "Vertex");
				cnv << sX;
				cnv >> x;
				cnv.str(std::string());
				cnv.clear();
				
				if (!getline(ss, sY, ';') || !is_All_Number(sY))
					throw Exception<string>("Error reading y", "Vertex");
				cnv << sY;
				cnv >> y;
				cnv.str(std::string());
				cnv.clear();
				
				if (!getline(ss, sType) || !is_All_Number(sType))
					throw Exception<string>("Error reading type", "Vertex");
				cnv << sType;
				cnv >> type;
				cnv.str(std::string());
				cnv.clear();
				
				// add intersection to the graph
				Intersection i(id, x, y, type);
				input.addVertex(i);
				
			} catch (Exception<string> &e) {
				// move to next line & cleanup before retry
				getline(ss, sType);
				cnv.str(std::string());
				cnv.clear();
				cout << "Failed to read vertex from line " << lineCounter << "\n";
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
	unsigned int lineCounter = 0;
	
	file.open(filePath);
	if (file.is_open()) {
		while (file.good()) {
			lineCounter++;
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
				if (!getline(ss, sSrcId, ';') || !is_All_Number(sSrcId))
					throw Exception<string>("Error reading source id", "Edges");
				cnv << sSrcId;
				cnv >> srcId;
				cnv.str(std::string());
				cnv.clear();
				
				if (!getline(ss, sDstId, ';') || !is_All_Number(sDstId))
					throw Exception<string>("Error reading destiny id",
											"Edges");
					cnv << sDstId;
				cnv >> dstId;
				cnv.str(std::string());
				cnv.clear();
				
				if (!getline(ss, sDist) || !is_All_Number(sDist))
					throw Exception<string>("Error reading distance", "Edges");
				cnv << sDist;
				cnv >> distance;
				cnv.str(std::string());
				cnv.clear();
				
				// find vertexes
				Intersection src(srcId, 0, 0, INTERSECTION);
				Intersection dst(dstId, 0, 0, INTERSECTION);
				
				// add edges to the graph
				input.addEdge(src, dst, distance);
				input.addEdge(dst, src, distance);

			} catch (Exception<string> &e) {
				// move to next line & cleanup before retry
				getline(ss, sDist);
				cnv.str(std::string());
				cnv.clear();
				cout << "Failed to read edge from line " << lineCounter << "\n";
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

void Algorithms::restoreResultGraph(){
	result.clear();
	result = input;
}

inline bool is_Not_Number(const int & c) {
	return !(c >= 48 && c <= 57);
}

bool is_All_Number(const string& s) {
	return find_if(s.begin(), s.end(), is_Not_Number) == s.end();
}
