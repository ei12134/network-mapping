#ifndef GRAPH_H_
#define GRAPH_H_

#include <cmath>
#include <float.h>
#include <iostream>
#include <limits.h>
#include <list>
#include <math.h>
#include <queue>
#include <vector>
using namespace std;

template<class T> class Edge;
template<class T> class Graph;

const int BEING_VISITED = 1;
const int DONE_VISITED = 2;
const int INT_INFINITY = INT_MAX;
const int NOT_VISITED = 0;
static const double PI = 3.141592653589793;

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template<class T>
class Vertex {
	T info;
	vector<Edge<T> > adj;
	bool processing;
	bool visited;
	double dist;
	int group;
public:
	Vertex(T in);
	T getInfo() const;
	vector<Edge<T> > getAdj() const;
	int getGroup();
	int getDist() const;
	friend class Graph<T> ;
	double distance(Vertex<T>* v);
	void addEdge(Vertex<T> *dest, double w);
	bool removeEdgeTo(Vertex<T> *d);
	vector<string> print(bool edges) const;
	Vertex* path;
	bool operator<(const Vertex<T> vertex);
};

template<class T>
double Vertex<T>::distance(Vertex<T>* v) {
	return info.distance(v->info);
}

template<class T>
Vertex<T>::Vertex(T in) :
		info(in), visited(false), dist(0), group(0) {
	path = NULL;
}

template<class T>
int Vertex<T>::getGroup() {
	return group;
}

template<class T>
T Vertex<T>::getInfo() const {
	return info;
}

template<class T>
int Vertex<T>::getDist() const {
	return this->dist;
}

template<class T>
vector<Edge<T> > Vertex<T>::getAdj() const {
	return adj;
}

template<class T>
struct vertex_greater_than {
	bool operator()(Vertex<T> * a, Vertex<T> * b) const {
		return a->getDist() > b->getDist();
	}
};

template<class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w) {
	Edge<T> edgeD(dest, w);
	edgeD.orig = this;
	adj.push_back(edgeD);
}

template<class T>
vector<string> Vertex<T>::print(bool edges) const {
	vector<string> p;
	if (edges) {
		for (size_t i = 0; i < adj.size(); i++) {
			stringstream ss;
			ss.clear();
			ss << "\t  " << info << "\t\t\t  " << (adj[i].dest)->info << "\t\t\t  " << adj[i].distance << " m";
			p.push_back(ss.str());
		}
	} else {
		stringstream ss;
		ss << "\t\t" << info.getId() << "\t\t" << info.getX() << "\t\t" << info.getY() << "\t\t" << info.getType();
		p.push_back(ss.str());
	}
	return p;
}

/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template<class T>
class Edge {
	Vertex<T> * dest;
	Vertex<T> * orig;
	double distance;
public:
	Edge(Vertex<T> *d, double w);
	Vertex<T> * getDest() const;
	Vertex<T> * getOrig() const;
	double getDistance() const;
	bool operator<(const Edge<T> &other) const;
	friend class Graph<T> ;
	friend class Vertex<T> ;
};

template<class T>
Edge<T>::Edge(Vertex<T> *d, double w) :
		dest(d), distance(w) {
}

template<class T>
Vertex<T> * Edge<T>::getDest() const {
	return dest;
}

template<class T>
Vertex<T> * Edge<T>::getOrig() const {
	return orig;
}

template<class T>
double Edge<T>::getDistance() const {
	return distance;
}

template<class T>
bool Edge<T>::operator<(const Edge<T> &other) const {
	return this->distance < other.distance;
}

template<class T>
struct edge_greater_than {
	bool operator()(Edge<T> a, Edge<T> b) const {
		return a.getDistance() > b.getDistance();
	}
};

template<class T>
struct group_greater_than {
	bool operator()(Vertex<T> *a, Vertex<T>* b) const {
		return a->getGroup() > b->getGroup();
	}
};

/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template<class T>
class Graph {
	vector<Vertex<T> *> vertexSet;
	vector<Vertex<T>*> centrals;
	int numCycles;
	Graph(vector<Vertex<T> *>);
public:
	Graph();
	Graph(const Graph<T>& g);
	vector<Vertex<T> *> getVertexSet() const;
	vector<Vertex<T>*> getCentrals() const;
	vector<string> print(bool edges) const;
	int getNumVertex() const;
	int getNumEdges() const;
	bool getConnected() const;
	double getTotalDistance() const;
	int maxNewChildren(Vertex<T> *v, T &inf) const;
	bool addVertex(const T & in);
	bool addEdge(const T & sourc, const T & dest, double w);
	bool removeVertex(const T &in);
	bool removeEdge(const T &sourc, const T &dest);

	vector<T> bfs(Vertex<T> *v) const;
	vector<T> dfs() const;
	void dfs(Vertex<T> *v, vector<T> &res) const;

	int getNumCycles();
	bool isDAG();
	void dfsVisit();
	void dfsVisit(Vertex<T> *v);
	void clear();
	double bfMst(int centralIndex, double area);
	vector<Vertex<T>*> calculatePrim();
	Graph<T> calculateKruskal(unsigned int num_centrals);
	void cleanIntersections();
	void setCentral();
	bool selectArea(double area);
	~Graph();
};

template<class T>
Graph<T>::Graph() {

}

template<class T>
Graph<T>::Graph(vector<Vertex<T> *> v) {
	vertexSet = v;
	for (size_t i = 0; i < vertexSet.size(); i++) {
		if(vertexSet[i]->info.getType() == CENTRAL)
			centrals.push_back(vertexSet[i]);
	}
}

template<class T>
Graph<T>::Graph(const Graph<T>& g) {
	// Copy vertexes
	for (size_t i = 0; i < g.vertexSet.size(); i++) {
		addVertex(g.vertexSet[i]->info);
	}
	// Copy edges
	for (size_t i = 0; i < g.vertexSet.size(); i++) {
		T source = g.vertexSet[i]->info;
		vector<Edge<T> > cpyAdj = g.vertexSet[i]->adj;

		for (size_t x = 0; x < cpyAdj.size(); x++) {
			T destiny = cpyAdj[x].dest->info;
			double distance = cpyAdj[x].distance;
			addEdge(source, destiny, distance);
		}
	}
}

template<class T>
void Graph<T>::clear() {
	typename vector<Vertex<T>*>::iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite = vertexSet.end();
	vertexSet.erase(it, ite);
}

template<class T>
Graph<T>::~Graph() {
	clear();
}

template<class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template<class T>
vector<Vertex<T>*> Graph<T>::getCentrals() const {
	return centrals;
}

template<class T>
int Graph<T>::getNumEdges() const {
	int edges = 0;
	for (size_t x = 0; x < vertexSet.size(); x++) {
		vector<Edge<Intersection> > adj = vertexSet[x]->getAdj();
		edges += adj.size();
	}
	return edges;
}

template<class T>
double Graph<T>::getTotalDistance() const {
	double distance = 0;

	for (size_t x = 0; x < vertexSet.size(); x++) {
		vector<Edge<Intersection> > adj = vertexSet[x]->getAdj();
		for (size_t i = 0; i < adj.size(); i++)
			distance += adj[i].getDistance();
	}

	return distance / 2; // undirected graph
}

template<class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

template<class T>
bool Graph<T>::getConnected() const {
	// only one vertex means it is conected
	if (vertexSet.size() < 2)
		return true;

	// make a bfs visit
	vector<T> vS = bfs(vertexSet[0]);
	if (vS.size() != vertexSet.size())
		return false;

	return true;
}

template<class T>
bool Graph<T>::addVertex(const T & in) {
	for (size_t i = 0; i < vertexSet.size(); i++)
		if (in.getId() == vertexSet[i]->info.getId())
			return false;

	Vertex<T> *v = new Vertex<T>(in);

	// Add central pointer
	if (v->info.getType() == CENTRAL)
		centrals.push_back(v);
	vertexSet.push_back(v);
	return true;
}

template<class T>
bool Graph<T>::addEdge(const T & sourc, const T & dest, double w) {
	int sourceIndex = -1;
	int destIndex = -1;
	for (size_t i = 0; i < vertexSet.size(); i++) {
		if (sourc.getId() == vertexSet[i]->info.getId())
			sourceIndex = i;
		else if (dest.getId() == vertexSet[i]->info.getId())
			destIndex = i;
	}
	if (sourceIndex < 0 || destIndex < 0)
		return false;

	for (size_t i = 0; i < vertexSet[sourceIndex]->adj.size(); i++)
		if (vertexSet[destIndex]->info.getId() == vertexSet[sourceIndex]->adj[i].dest->info.getId())
			return false;

	vertexSet[sourceIndex]->addEdge(vertexSet[destIndex], w);
	return true;
}

template<class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
	typename vector<Edge<T> >::iterator it = adj.begin();
	typename vector<Edge<T> >::iterator ite = adj.end();
	while (it != ite) {
		if (it->dest == d) {
			adj.erase(it);
			return true;
		} else
			it++;
	}
	return false;
}

template<class T>
bool Graph<T>::removeVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite = vertexSet.end();
	for (; it != ite; it++) {
		if ((*it)->info == in) {
			Vertex<T> * v = *it;
			typename vector<Vertex<T>*>::iterator it1 = vertexSet.begin();
			typename vector<Vertex<T>*>::iterator it1e = vertexSet.end();
			for (; it1 != it1e; it1++) {
				// remove edges having input as destiny
				(*it1)->removeEdgeTo(v);
				// remove edges having input as source
				v->removeEdgeTo((*it1));
			}
			vertexSet.erase(it);
			delete v;
			return true;
		}
	}
	return false;
}

template<class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	typename vector<Vertex<T>*>::iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite = vertexSet.end();
	int found = 0;
	Vertex<T> *vS, *vD;
	while (found != 2 && it != ite) {
		if ((*it)->info == sourc) {
			vS = *it;
			found++;
		}
		if ((*it)->info == dest) {
			vD = *it;
			found++;
		}
		it++;
	}
	if (found != 2)
		return false;

	return vS->removeEdgeTo(vD);
}

template<class T>
vector<string> Graph<T>::print(bool edges) const {
	vector<string> p;

	for (size_t i = 0; i < vertexSet.size(); i++) {
		vector<string> v = vertexSet[i]->print(edges);
		for (size_t j = 0; j < v.size(); j++)
			p.push_back(v[j]);
		v.clear();
	}

	return p;
}

template<class T>
int Graph<T>::getNumCycles() {
	numCycles = 0;
	dfsVisit();
	return this->numCycles;
}

template<class T>
bool Graph<T>::isDAG() {
	if (getNumCycles() == 0)
		return true;
	return false;
}

template<class T>
void Graph<T>::dfsVisit() {
	typename vector<Vertex<T>*>::const_iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite = vertexSet.end();
	for (; it != ite; it++)
		(*it)->visited = false;
	it = vertexSet.begin();
	for (; it != ite; it++)
		if ((*it)->visited == false) {
			cout << "Being visited" << endl;
			dfsVisit(*it);
		}
}

template<class T>
void Graph<T>::dfsVisit(Vertex<T> *v) {
	v->processing = true;
	v->visited = true;
	typename vector<Edge<T> >::iterator it = (v->adj).begin();
	typename vector<Edge<T> >::iterator ite = (v->adj).end();
	for (; it != ite; it++) {
		if (it->dest->processing == true)
			numCycles++;
		if (it->dest->visited == false) {
			dfsVisit(it->dest);
		}
	}
	v->processing = false;
}

template<class T>
vector<T> Graph<T>::dfs() const {
	typename vector<Vertex<T>*>::const_iterator it = vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite = vertexSet.end();
	for (; it != ite; it++)
		(*it)->visited = false;
	vector<T> res;
	it = vertexSet.begin();
	for (; it != ite; it++)
		if ((*it)->visited == false)
			dfs(*it, res);
	return res;
}

template<class T>
void Graph<T>::dfs(Vertex<T> *v, vector<T> &res) const {
	v->visited = true;
	res.push_back(v->info);
	typename vector<Edge<T> >::iterator it = (v->adj).begin();
	typename vector<Edge<T> >::iterator ite = (v->adj).end();
	for (; it != ite; it++)
		if (it->dest->visited == false) {
			dfs(it->dest, res);
		}
}

template<class T>
vector<T> Graph<T>::bfs(Vertex<T> *v) const {
	vector<T> res;
	queue<Vertex<T> *> q;
	q.push(v);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		typename vector<Edge<T> >::iterator it = v1->adj.begin();
		typename vector<Edge<T> >::iterator ite = v1->adj.end();
		for (; it != ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited == false) {
				d->visited = true;
				q.push(d);
			}
		}
	}
	return res;
}

template<class T>
bool Graph<T>::selectArea(double area) {
	if (centrals.size() < 1 || area == DBL_MAX)
		return false;

	double radius = sqrt(abs((area / 2.0) / PI));

	// Remove all vertexes and edges outside the radius
	for (int i = 0; i < (int) vertexSet.size(); i++) {
		bool remove = true;
		// Check distance to each central
		for (int x = 0; x < (int) centrals.size(); x++) {
			Vertex<T>* central = centrals[x];
			if (vertexSet[i]->info.distance(central->info) <= radius)
				remove = false;
		}
		if (remove && removeVertex(vertexSet[i]->info))
			i--;
	}

	// Remove vertexes that ended up without connection
	for (int i = 0; i < (int) vertexSet.size(); i++) {
		if (vertexSet[i]->adj.size() == 0) {
			removeVertex(vertexSet[i]->info);
			i--;
		}
	}

	return true;
}

template<class T>
vector<Vertex<T>*> Graph<T>::calculatePrim() {
	for (unsigned int i = 0; i < this->vertexSet.size(); i++) {
		this->vertexSet[i]->path = NULL;
		this->vertexSet[i]->dist = INT_INFINITY;
		this->vertexSet[i]->visited = false;
	}

	Vertex<T>* v = this->vertexSet[0];
	v->dist = 0;

	vector<Vertex<T>*> pq;
	pq.push_back(v);

	make_heap(pq.begin(), pq.end(), vertex_greater_than<T>());

	// a kind of Dijkstra modified
	while (!pq.empty()) {

		v = pq.front();
		pop_heap(pq.begin(), pq.end());
		pq.pop_back();
		if (!v->visited) {
			v->visited = true;
			for (unsigned int i = 0; i < v->adj.size(); i++) {

				Vertex<T>* w = v->adj[i].dest;

				if (!w->visited) {
					double temp = v->adj[i].distance;
					if (temp < w->dist) {
						w->dist = v->adj[i].distance;
						w->path = v;
						pq.push_back(w);
					}
				}
			}
		}
		make_heap(pq.begin(), pq.end(), vertex_greater_than<T>());
	}

	// clean redundant intersections
	bool modified = false;

	// as long as we removed an intersection keep searching for new ones
	do {
		modified = false;
		for (unsigned int i = 0; i < vertexSet.size(); i++) {
			if (vertexSet[i]->info.getType() == INTERSECTION) {
				bool remove = true;
				for (unsigned int x = 0; x < vertexSet.size(); x++) {
					if ((x != i) && (vertexSet[x]->path == vertexSet[i])) {
						remove = false;
						break;
					}
				}
				if (remove) {
					removeVertex(vertexSet[i]->info);
					modified = true;
				}
			}
		}
	} while (modified);

	return this->vertexSet;
}

template<class T>
void Graph<T>::cleanIntersections() {
	bool reCheck;
	// Clean redundant intersections
	do {
		reCheck = false;
		for (unsigned int i = 0; i < vertexSet.size(); i++) {
			if (vertexSet[i]->info.getType() == INTERSECTION) {
				if (vertexSet[i]->adj.size() == 0) {
					delete vertexSet[i];
					vertexSet.erase(vertexSet.begin() + i);
					i--;
					reCheck = true;
				} else if (vertexSet[i]->adj.size() == 1) { // intersection alone
					Vertex<T>* v = vertexSet[i]->adj[0].dest;
					for (unsigned int x = 0; x < v->adj.size(); x++)
						if (v->adj[x].dest == vertexSet[i]) {
							v->adj.erase(v->adj.begin() + x);
							break;
						}
					delete vertexSet[i];
					vertexSet.erase(vertexSet.begin() + i);
					i--;
					reCheck = true;
				}
			}
		}
	} while (reCheck);
}

template<class T>
void Graph<T>::setCentral() {
	if (centrals.size() > 0 || vertexSet.size() < 1)
		return;

	sort(vertexSet.begin(), vertexSet.end(), group_greater_than<T>());

	vector<int> vecStartPos;
	vecStartPos.push_back(0);
	unsigned int num_centrals = 1;
	for (unsigned x = 1; x < vertexSet.size(); x++) {
		if (vertexSet[x - 1]->group != vertexSet[x]->group) {
			vecStartPos.push_back(x);
			num_centrals++;
		}
	}
	vecStartPos.push_back((int) vertexSet.size());

	cout << " num_centrals -> " << num_centrals << endl;

	unsigned int temp = 0;
	Vertex<T>* v;
	while (temp < num_centrals) {
		cout << " iteration -> " << temp << endl;
		double distMin = DBL_MAX;
		double maxDist = 0;
		v = vertexSet[vecStartPos[temp]];
		for (int x = vecStartPos[temp]; x < vecStartPos[temp + 1] - 1; x++) {
			maxDist = 0;
			for (int y = vecStartPos[temp] + 1; y < vecStartPos[temp + 1]; y++) {
				double tempDist = vertexSet[x]->distance(vertexSet[y]);
				if (tempDist > maxDist)
					maxDist = tempDist;
			}
			if (distMin > maxDist) {
				distMin = maxDist;
				v = vertexSet[x];
			}
		}
		v->info.setType(CENTRAL);
		centrals.push_back(v);
		temp++;
	}
}

template<class T>
Graph<T> Graph<T>::calculateKruskal(unsigned int num_centrals) {
	vector<Vertex<T>*> finalVec;

	if (num_centrals == 0 || vertexSet.size() == 0)
		return finalVec; // returns empty vector in case of error

	unsigned edges_accepted = 0;

	//list of edges
	vector<Edge<T> > allEdges;
	for (unsigned int i = 0; i < this->vertexSet.size(); i++) {
		Vertex<T>* v = this->vertexSet[i];
		Vertex<T>* ver = new Vertex<T>(v->info);
		ver->group = i;
		finalVec.push_back(ver);
		v->group = i;
		for (unsigned int a = 0; a < v->adj.size(); a++)
			allEdges.push_back(v->adj[a]);
	}

	make_heap(allEdges.begin(), allEdges.end(), edge_greater_than<T>());

	while (edges_accepted < vertexSet.size() - num_centrals) {
		sort_heap(allEdges.begin(), allEdges.end());

		Edge<T> minEdge = allEdges[0];		// get edge with minimum weight
		allEdges.erase(allEdges.begin());

		//Get the vertices
		T o = minEdge.orig->info;
		T d = minEdge.dest->info;

		//Vertex<T>* origin = NULL;
		//Vertex<T>* dest = NULL;

		int i_O;
		int i_D;
		for (unsigned int i = 0; i < finalVec.size(); i++) {
			if (o == finalVec[i]->info) {
				i_O = i;
			}
			if (d == finalVec[i]->info) {
				i_D = i;
			}
		}

		if (finalVec[i_O]->group != finalVec[i_D]->group) {		// para n ao haver ciclos
			int minSet = min(finalVec[i_O]->group, finalVec[i_D]->group);
			int maxSet = max(finalVec[i_O]->group, finalVec[i_D]->group);
			for (unsigned int k = 0; k < finalVec.size(); k++) { // change group of max to min(check if there is any diference without this)
				if (finalVec[k]->group == maxSet) {
					finalVec[k]->group = minSet;
				}
			}
			// assim para todos os vertices terem o numero certo de arestas
			finalVec[i_O]->addEdge(finalVec[i_D], minEdge.getDistance());
			finalVec[i_D]->addEdge(finalVec[i_O], minEdge.getDistance());
			edges_accepted++;
			cout << "Adding edge from vertex " << finalVec[i_O]->info << " to vertex " << finalVec[i_D]->info << endl;
		}
	}

	
	Graph g = Graph<T>(finalVec);
	g.setCentral();
	// cleans redundant intersection
	g.cleanIntersections();

	return g;
}

#endif // GRAPH_H_
