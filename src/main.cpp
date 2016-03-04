#include "graph.hpp"

using namespace graph;

int main( void )
{
	cout << "construct graph" << endl;
    Graph graph(6);
    graph.siblings(0);
	cout << "choose vertex 0" << endl;
	Vertex* v = graph[0];
	cout << "traverse graph until end" << endl;
	graph.traverse(v);
	graph.graphviz("/home/letrend/workspace/graph/graph.png");

	cout << "add edge between vertex 0 and 5" << endl;
	graph.addEdge(0,5);
	graph.traverse(v);

	cout << "remove edge between vertex 0 and 1" << endl;
	graph.removeEdge(0,1);
	graph.traverse(v);

	std::cout << "new graph" << std::endl;
	Graph graph2(10);
	graph2.addEdge(0,9);
	graph2.addEdge(1,9);
	graph2.addEdge(2,9);
	graph2.addEdge(3,9);
	graph2.addEdge(4,9);
	graph2.addEdge(5,9);
	graph2.addEdge(6,9);
	graph2.addEdge(7,9);
	graph2.addEdge(3,5);
	graph2.addEdge(2,5);
	graph2.addEdge(1,5);

	v = graph2[0];
	graph2.traverse(v);
	std::cout << "merge vertex 1 and 2" << std::endl;
	graph2.graphviz("/home/letrend/workspace/graph/graph_unmerged.png");
	graph2.mergeVertices(1,2);
	graph2.mergeVertices(1,3);
	graph2.mergeVertices(1,4);
	graph2.mergeVertices(1,5);
	graph2.mergeVertices(1,6);
	graph2.mergeVertices(1,7);
	graph2.mergeVertices(1,8);
	graph2.mergeVertices(0,1);
	graph2.graphviz("/home/letrend/workspace/graph/graph_merged.png");

    return 0;
}
