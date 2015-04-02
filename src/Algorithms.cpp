#include "Algorithms.h"

Algorithms::Algorithms() {

}

void Algorithms::loadData(const char * vertexFileName,
		const char * edgesFileName) {
	graph.clear();
	readVertexes(vertexFileName);
	readEdges(edgesFileName);
}

void Algorithms::saveData() {

}

void Algorithms::readVertexes(const char *filePath) {
	fstream file;
	string line, sId, sType;
	stringstream ss, cnv;
	unsigned int id;
	unsigned char type;

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
				cnv.clear();

				if (!getline(ss, sType))
					throw Exception<string>("Error reading type", "Vertex");
				cnv << sType;
				cnv >> type;

				// add intersection to the graph
				Intersection i(id, type);
				graph.addVertex(i);
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
				Intersection src(srcId, INTERSECTION);
				Intersection dst(dstId, INTERSECTION);

				// add edge to the graph
				graph.addEdge(src, dst, distance);
			} catch (Exception<string> &e) {
			}
		}
	}
	file.close();
}

void Algorithms::print(){
	graph.print();
}
