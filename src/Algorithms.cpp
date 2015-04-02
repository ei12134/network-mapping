/*
 * Algorithms.cpp
 */

#include "Algorithms.h"

void Algorithms::loadData(const char * vertexPath,const char * edgesPath) {
	graph.clean();
	readVertex(vertexPath);
	readEdges(edgesPath);
}

void Algorithms::saveData() {

}

bool Algorithms::readVertex(const char *filePath) {
	fstream file;
	string line;
	stringstream ss;

	file.open(filePath);
	if (file.is_open()) {
		while (file.good()) {
			try {
				ss.clear();
				getline(file, line, '\n');
				ss << line;



				/*Book* bk = new Book(ss);
				books.push_back(bk);
				if (!bk->getDeleted())
					booksTree.insert(bk);*/
			} catch (Exception<string> &e) {
			}
		}
	}
	file.close();
	return true;
}

bool Algorithms::readEdges(const char *filePath) {
	return true;
}
