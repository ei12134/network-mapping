#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>

using namespace std;

template<class T> class Edge;
template<class T> class Graph;

template<class T>
class Vertex {
	T info;
	vector<Edge<T> > adj;
	bool visited;
public:
	Vertex(T in);
	friend class Graph<T> ;
	void addEdge(Vertex<T> *dest, double w);
	bool removeEdge(Vertex<T> *dest);
	vector<string> print(bool edges) const;

	friend class Cli;
};

template<class T>
class Edge {
	Vertex<T> * dest;
	double distance;
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T> ;
	friend class Vertex<T> ;

	friend class Cli;
};

template<class T>
class Graph {
	vector<Vertex<T> *> vertexSet;
public:
	vector<Vertex<T> *> getVertexSet() const;
	vector<T> dfs() const;
	vector<T> bfs(Vertex<T> *v) const;
	vector<string> print(bool edges) const;
	int getNumVertex() const;
	int maxNewChildren(Vertex<T> *v, T &inf) const;
	bool addVertex(const T & in);
	bool addEdge(const T & sourc, const T & dest, double w);
	bool removeVertex(const T &in);
	bool removeEdge(const T &sourc, const T &dest);
	bool acyclic() const;
	void acyclic(Vertex<T>* v, bool& cycle) const;
	void dfs(Vertex<T> *v, vector<T> & dS) const;
	void clear();
	~Graph();
};

#include "Graph.h"

template<class T>
Vertex<T>::Vertex(T in) :
		info(in), visited(false) {
}

template<class T>
bool Vertex<T>::removeEdge(Vertex<T> *dest) {
	for (size_t i = 0; i < adj.size(); i++) {
		if (adj[i].dest->info.getId() == dest->info.getId()) {
			adj.erase(adj.begin() + i);
			return true;
		}
	}
	return false;
}

template<class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w) {
	Edge<T> edgeD(dest, w);
	adj.push_back(edgeD);
}

template<class T>
vector<string> Vertex<T>::print(bool edges) const {
    vector<string> p;
    if (edges){
	for (size_t i = 0; i < adj.size(); i++) {
	    stringstream ss;
	    ss.clear();
	    ss << "\t  " << info << "\t\t\t  " << (adj[i].dest)->info << "\t\t\t  "<< adj[i].distance << " m";
	    p.push_back(ss.str());
	}
    }
    else{
	stringstream ss;
	ss << "\t\t" << info.getId() << "\t\t" << info.getX() << "\t\t"<< info.getX() << "\t\t" << info.getType();
	p.push_back(ss.str());
    }
    return p;
}

template<class T>
Edge<T>::Edge(Vertex<T> *d, double w) :
		dest(d), distance(w) {
}

template<class T>
void Graph<T>::clear() {
// 	for (unsigned x = 0; x < vertexSet.size(); x++) {
// 		delete vertexSet[x];
// 	}
// 	vertexSet.clear();
     typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
    typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
    vertexSet.erase(it, ite);
}

template<class T>
Graph<T>::~Graph() {
	//clear(); RECHECK !!
    clear();
}

template<class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template<class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

template<class T>
bool Graph<T>::addVertex(const T & in) {
	for (size_t i = 0; i < vertexSet.size(); i++)
		if (in.getId() == vertexSet[i]->info.getId())
			return false;

	Vertex<T> *v = new Vertex<T>(in);
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
		if (vertexSet[destIndex]->info.getId()
				== vertexSet[sourceIndex]->adj[i].dest->info.getId())
			return false;

	vertexSet[sourceIndex]->addEdge(vertexSet[destIndex], w);
	return true;
}

template<class T>
bool Graph<T>::removeVertex(const T &in) {
	int removeIndex = -1;
	for (size_t i = 0; i < vertexSet.size(); i++) {
		if (in.getId() == vertexSet[i]->info.getId()) {
			removeIndex = i;
			break;
		}
	}
	if (removeIndex != -1) {
		for (size_t j = 0; j < vertexSet.size(); j++) {
			if (j != removeIndex) {
				vertexSet[j]->removeEdge(vertexSet[removeIndex]);
			}
		}
		vertexSet.erase(vertexSet.begin() + removeIndex);
		return true;
	} else
		return false;
}

template<class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	int sourceIndex = -1;
	int destIndex = -1;

	for (size_t i = 0; i < vertexSet.size(); i++) {
		if (sourc.getId() == vertexSet[i]->info.getId()) {
			sourceIndex = i;
		} else if (dest.getId() == vertexSet[i]->info.getId()) {
			destIndex = i;
		}
	}

	if (sourceIndex != -1 && destIndex != -1)
		return vertexSet[sourceIndex]->removeEdge(vertexSet[destIndex]);
	else
		return false;
}

template<class T>
bool Graph<T>::acyclic() const{
    bool cycle = false;
    
    for (size_t i = 0; i < vertexSet.size(); i++) {
	vertexSet[i]->visited = false;
    }
    for (size_t i = 0; i < vertexSet.size(); i++) {
	if (!vertexSet[i]->visited){
	    acyclic(vertexSet[i],cycle);
	}
    }
    return !cycle;
}

template<class T>
void Graph<T>::acyclic(Vertex<T>* v, bool& cycle) const {
    v->visited = true;
    for (size_t i = 0; i < v->adj.size(); i++) {
	if (!v->adj[i].dest->visited){
		acyclic(v->adj[i].dest,cycle);
		if (cycle)
			return;
	}
	else{
	    cycle = true;
		return;
    }
	}
}

template<class T>
vector<T> Graph<T>::dfs() const {
    vector<T> dS;
    for (size_t i = 0; i < vertexSet.size(); i++) {
	vertexSet[i]->visited = false;
    }
    for (size_t i = 0; i < vertexSet.size(); i++) {
	if (!vertexSet[i]->visited)
	    dfs(vertexSet[i], dS);
    }
    return dS;
}

template<class T>
void Graph<T>::dfs(Vertex<T>* v, vector<T>& dS) const {
    v->visited = true;
    dS.push_back(v->info);
    for (size_t i = 0; i < v->adj.size(); i++) {
	if (!v->adj[i].dest->visited)
	    dfs(v->adj[i].dest, dS);
    }
}

template<class T>
vector<T> Graph<T>::bfs(Vertex<T> *v) const {
	vector<T> bS;
	queue<Vertex<T>*> q;
	q.push(v);
	v->visited = true;

	while (!q.empty()) {
		Vertex<T>* v = q.front();
		q.pop();
		bS.push_back(v->info);

		// find vertexes adjacent to v and adds them to the priority queue
		for (size_t i = 0; i < v->adj.size(); i++) {
			Vertex<T>* w = v->adj[i].dest;
			if (!w->visited) {
				w->visited = true;
				q.push(w);
			}
		}
		v->visited = true;
	}
	return bS;
}

template<class T>
int Graph<T>::maxNewChildren(Vertex<T> *v, T &inf) const {
	vector<T> res;
	queue<Vertex<T> *> q;
	queue<int> level;
	int maxChildren = 0;
	inf = v->info;
	q.push(v);
	level.push(0);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		int l = level.front();
		level.pop();
		l++;
		int nChildren = 0;
		typename vector<Edge<T> >::iterator it = v1->adj.begin();
		typename vector<Edge<T> >::iterator ite = v1->adj.end();
		for (; it != ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited == false) {
				d->visited = true;
				q.push(d);
				level.push(l);
				nChildren++;
			}
		}
		if (nChildren > maxChildren) {
			maxChildren = nChildren;
			inf = v1->info;
		}
	}
	return maxChildren;
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
#endif
