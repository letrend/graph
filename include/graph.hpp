#pragma once
#include <vector>
#include <map>
#include <iostream>

using std::map;
using std::vector;
using std::cout;
using std::endl;

namespace graph {
	struct Vertex;
	struct Edge;

	struct Vertex {
		uint index;
		vector<Edge> edges_in, edges_out;
	};
	struct Edge {
		Vertex *from, *to;
	};

	class Graph {
	public:
		Graph(uint numberVertices) {
			addVertex();
			Edge edge;
			edge.from = vertices[0]; // first vertex points to itself
			edge.to = vertices[0];
			totalNrEdges++;
			vertices[0]->edges_in.push_back(edge);
			vertices[0]->index = 0;
			indexVertex[0] = vertices[0];
			for (uint i = 1; i < numberVertices; i++) {
				addVertex();
				edge.from = vertices[i - 1];
				edge.to = vertices[i];
				totalNrEdges++;
				vertices[i - 1]->edges_out.push_back(edge);
				vertices[i]->edges_in.push_back(edge);
				indexVertex[i] = vertices[i];
				vertices[i]->index = i;
			}
			edge.from = vertices.back(); // last vertex points to itself
			edge.to = vertices.back();
			totalNrEdges++;
			vertices.back()->edges_out.push_back(edge);
		}

		void siblings(uint from) {
			if (indexVertex.find(from) != indexVertex.end()) {
				Vertex *v = indexVertex.at(from);
				cout << "vertex " << from << " siblings: \n";
				for (auto edge:v->edges_out) {
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
			if (from == from->edges_out[0].to)
				return;
			for (auto edge:from->edges_out) {
				cout << "(" << from->index << " -> " << edge.to->index << ")" << endl;
				traverse(edge.to);
			}
		}

		void addEdge(uint from, uint to) {
			if (indexVertex.find(from) != indexVertex.end() && indexVertex.find(to) != indexVertex.end()) {
				Vertex *v0 = indexVertex[from], *v1 = indexVertex[to];
				Edge edge;
				edge.from = v0;
				edge.to = v1;
				totalNrEdges++;
				v0->edges_out.push_back(edge);
				v1->edges_in.push_back(edge);
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
				edge.from = v0;
				edge.to = v1;
				if (checkIfEdgeExists(v0->edges_out, edge, edge2delete))
					v0->edges_out.erase(edge2delete);
				if (checkIfEdgeExists(v1->edges_out, edge, edge2delete))
					v1->edges_out.erase(edge2delete);
				if (checkIfEdgeExists(v0->edges_in, edge, edge2delete))
					v0->edges_in.erase(edge2delete);
				if (checkIfEdgeExists(v1->edges_in, edge, edge2delete))
					v1->edges_in.erase(edge2delete);
			} else {
				cout << "ERROR: vertex " << from << " or " << to << " does not exist in graph" << endl;
			}
		}

		inline bool checkIfEdgeExists(vector<Edge> &a, Edge &b, vector<Edge>::iterator &edge) {
			for (auto it = a.begin(); it != a.end(); ++it) {
				if (it->to->index == b.to->index && it->from->index == b.from->index) {
					edge = it;
					return true;
				}
			}
			return false;
		}

		void removeVertex(uint v) {
			auto it = indexVertex.find(v);
			if (it != indexVertex.end()) {
				Vertex *v0 = indexVertex[v];
				// connect all vertices connected by the vertex to be deleted
				for (auto from:v0->edges_in) {
					for (auto to:v0->edges_out) {
						addEdge(from.from->index, to.to->index);
					}
				}
				// delete all edges to siblings and from parents
				for (auto from:v0->edges_in) {
					removeEdge(from.from->index, v0->index);
				}
				for (auto to:v0->edges_out) {
					removeEdge(v0->index, to.to->index);
				}
				// add edge to itself if it is the first or last vertex
				if (v0->edges_in.empty() || v0->edges_out.empty()) {
					addEdge(v0->index, v0->index);
				}
				// erase vertex
				indexVertex.erase(it);
				for (auto it = vertices.begin(); it != vertices.end(); ++it) {
					if ((*it)->index == v) {
						// clear the data
						delete (*it);
						vertices.erase(it);
						return;
					}
				}
			} else {
				cout << "ERROR: vertex " << v << " does not exist in graph" << endl;
			}
		}

		void mergeVertices(uint v, uint w){
			if (indexVertex.find(v) != indexVertex.end() && indexVertex.find(w) != indexVertex.end()) {
				Vertex *v0 = indexVertex.at(v), *v1 = indexVertex.at(w);
				// adjust indices of vertex w
				for (auto edge:v1->edges_in) {
					if(edge.from!=v0) {
						edge.to = v0;
						for(auto it = edge.from->edges_out.begin(); it!=edge.from->edges_out.end(); ++it){
							if(it->to == v1) {
								it->to = v0;
								break;
							}
						}
					}else{
						removeEdge(edge.from->index,v1->index);
					}
				}
				for (auto edge:v1->edges_out) {
					if(edge.to!=v0 && edge.to!=v1)
						edge.from = v0;
					else
						removeEdge(v1->index,v1->index);
				}
				// merge edges_in and edges_out
				v0->edges_in.insert( v0->edges_in.end(), v1->edges_in.begin(), v1->edges_in.end() );
				v0->edges_out.insert( v0->edges_out.end(), v1->edges_out.begin(), v1->edges_out.end() );
				if(v0->edges_out.empty()) { // terminate vertex
					Edge edge;
					edge.from = v0;
					edge.to = v0;
					v0->edges_out.push_back(edge);
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

	private:
		map<uint, Vertex *> indexVertex;
		vector<Vertex*> vertices;
		uint totalNrEdges = 0;
	};
}
