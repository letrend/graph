#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

using std::map;
using std::vector;
using std::cout;
using std::endl;

namespace graph {
	struct Vertex;
	struct Edge;

	struct Vertex {
		uint index;
		vector<Edge> edges;
	};
	struct Edge {
		Vertex *to;
	};

	class Graph {
	public:
		Graph(uint numberVertices) {
			addVertex();
			Edge edge;
			for (uint i = 0; i < numberVertices-1; i++) {
				addVertex();
				edge.to = vertices[i+1];
				totalNrEdges++;
				vertices[i]->edges.push_back(edge);
				indexVertex[i] = vertices[i];
				vertices[i]->index = i;
			}
		}

		void siblings(uint from) {
			if (indexVertex.find(from) != indexVertex.end()) {
				Vertex *v = indexVertex.at(from);
				cout << "vertex " << from << " siblings: \n";
				for (auto edge:v->edges) {
					if (edge.to == v)
						cout << "none";
					else
						cout << edge.to->index << " ";
				}
				cout << endl;
			} else {
				cout << "ERROR: vertex " << from << " does not exist in graph" << endl;
			}
		}

		void traverse(Vertex *from) {
			if (from->edges.empty())
				return;
			for (auto edge:from->edges) {
				cout << "(" << from->index << " -> " << edge.to->index << ")" << endl;
				traverse(edge.to);
			}
		}

		template<typename F>
		void traverse(Vertex *from, F f) {
			if (from == from->edges[0].to)
				return;
			for (auto edge:from->edges) {
				f(from);
				traverse(edge.to,f);
			}
		}

		void addEdge(uint from, uint to) {
			if (indexVertex.find(from) != indexVertex.end() && indexVertex.find(to) != indexVertex.end()) {
				Vertex *v0 = indexVertex[from], *v1 = indexVertex[to];
				Edge edge;
				edge.to = v1;
				totalNrEdges++;
				v0->edges.push_back(edge);
			} else {
				cout << "ERROR: vertex " << from << " or " << to << " does not exist in graph" << endl;
			}
		}

		void addVertex(){
			Vertex *v = new Vertex;
			v->index = vertices.size();
			vertices.push_back(v);
			indexVertex[v->index] = vertices[v->index];
		}

		void removeEdge(uint from, uint to) {
			if (indexVertex.find(from) != indexVertex.end() && indexVertex.find(to) != indexVertex.end()) {
				Vertex *v0 = indexVertex.at(from), *v1 = indexVertex.at(to);
				totalNrEdges--;
				Edge edge;
				vector<Edge>::iterator edge2delete;
				edge.to = v1;
				if (checkIfEdgeExists(v0->edges, edge, edge2delete))
					v0->edges.erase(edge2delete);
			} else {
				cout << "ERROR: vertex " << from << " or " << to << " does not exist in graph" << endl;
			}
		}

		inline bool checkIfEdgeExists(vector<Edge> &a, Edge &b, vector<Edge>::iterator &edge) {
			for (auto it = a.begin(); it != a.end(); ++it) {
				if (it->to->index == b.to->index) {
					edge = it;
					return true;
				}
			}
			return false;
		}

		void mergeVertices(uint v, uint w){
			if (indexVertex.find(v) != indexVertex.end() && indexVertex.find(w) != indexVertex.end()) {
				Vertex *v0 = indexVertex.at(v), *v1 = indexVertex.at(w);
				// delete all edges between v0 and v1
				vector<Edge> edges2delete;
				for (auto edge:v0->edges) {
					if(edge.to==v1) {
						edges2delete.push_back(edge);
					}
				}
				for(auto edge:edges2delete){
					vector<Edge>::iterator edge2delete;
					if (checkIfEdgeExists(v0->edges, edge, edge2delete))
						v0->edges.erase(edge2delete);
				}
				// copy all edges_out to v0
				for (auto edge:v1->edges) {
					if(edge.to!=v0) {
						v0->edges.push_back(edge);
					}
				}
				// if it is the last vertex terminate it
				if(v0->edges.empty()) {
					Edge edge;
					edge.to = v0;
					v0->edges.push_back(edge);
				}
				// erase vertex w
				auto it = indexVertex.find(w);
				indexVertex.erase(it);
				for (auto it = vertices.begin(); it != vertices.end(); ++it) {
					if ((*it)->index == w) {
						// clear the data
						delete (*it);
						vertices.erase(it);
						return;
					}
				}
			} else {
				cout << "ERROR: vertex " << v << " or " << w << " does not exist in graph" << endl;
			}
		}

		Vertex *operator[](uint i) {
			auto it = indexVertex.find(i);
			if (it != indexVertex.end()) {
				return indexVertex[i];
			} else {
				cout << "ERROR: vertex " << i << " does not exist in graph" << endl;
			}
		}

		void graphviz(const char* file = "graph.png"){
			std::stringstream str;
			str << "Graph g{\n";
			for(auto it:vertices){
				str << it->index << ";\n";
				for(auto edge:it->edges){
					str << it->index << "--" << edge.to->index << ";\n";
				}
			}
			str << "}" << endl;
			char cmd[2000];
			sprintf(cmd,"echo \"%s\"|dot -Tpng -o%s", str.str().c_str(), file);
			system(cmd);
		}

	private:
		map<uint, Vertex *> indexVertex;
		vector<Vertex*> vertices;
		uint totalNrEdges = 0;
	};
}
