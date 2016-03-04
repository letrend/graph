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

	cout << "add edge between vertex 0 and 5" << endl;
	graph.addEdge(0,5);
	graph.traverse(v);

	cout << "remove edge between vertex 0 and 1" << endl;
	graph.removeEdge(0,1);
	graph.traverse(v);

	std::cout << "remove vertex 5" << std::endl;
	graph.removeVertex(5);
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

	v = graph2[0];
	graph2.traverse(v);
	std::cout << "merge vertex 1 and 2" << std::endl;
	graph2.graphviz();
	graph2.mergeVertices(1,2);
	graph2.traverse(v);
	graph2.graphviz();



    return 0;
}
