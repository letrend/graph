#include "graph.hpp"

using namespace graph;

int main( void )
{
	cout << "construct graph" << endl;
    Graph graph(8);
	graph.addEdge(0,4);
	graph.addEdge(0,1);
	graph.addEdge(1,2);
	graph.addEdge(2,3);
	graph.addEdge(4,5);
	graph.addEdge(5,6);
	graph.addEdge(6,7);

	Vertex *v = graph[0];
	graph.traverse(v);
	std::cout << "merge vertex 1 and 4" << std::endl;
	graph.graphviz("/home/letrend/workspace/graph/graph_unmerged.png");
	graph.mergeVertices(0,4);
	graph.mergeVertices(2,6);
	graph.graphviz("/home/letrend/workspace/graph/graph_merged.png");

    return 0;
}
