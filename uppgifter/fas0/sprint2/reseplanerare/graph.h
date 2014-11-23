/** 
 *  @file	graph.h
 */

typedef struct graph Graph;
typedef struct node Node;
typedef struct edge Edge;

/**@brief Creates a graph with nodes and edges
 * @param e edges to add to the graph
 * @param s number of edges
 * @return the new graph
 */

Graph *mkGraph(Edge**, int);

/**@brief removes the graph and all its nodes and edges
 * @param graph the graph to remove
 */

void rmGraph(Graph*);
int graphSize(Graph*);
Edge **graphEdges(Graph*);
/**@brief creates a node with the given value
 * @param p the value of the new node
 * @return the new node
 */
Node *mkNode(void*);
void rmNode(Node*, Graph*);
/**@brief creates an edge with two nodes, a cost and a value
 * @param firstNode the first node
 * @param secondNode the second node
 * @param cost the cost it takes to go via the edge
 * @param p the value of the edge
 * @return the new edge
 */
Edge *mkEdge(Node*, Node*, int, void*);
void rmEdge(Edge*, Graph*);
void *getNodeValue(Node*);
void setNodeValue(Node*, void*);
/**@brief gets the first node connected to the edge
 * @param edge the edge to get the node from
 * @return the first node of the edge
 */
Node *getEdgeFirst(Edge*);
/**@brief gets the second node connected to the edge
 * @param edge the edge to get the node from
 * @return the second node of the edge
 */
Node *getEdgeSecond(Edge*);
/**@brief sets the first node connected to the edge
 * @param edge the edge to set the node on
 * @param node the node to connect to the edge
 */
void setEdgeFirst(Edge*, Node*);
/**@brief sets the second node connected to the edge
 * @param edge the edge to set the node on
 * @param node the node to connect to the edge
 */
void setEdgeSecond(Edge*, Node*);
int getEdgeCost(Edge*);
void *getEdgeValue(Edge*);
void setEdgeValue(Edge*, void*);
Edge *nextEdge();
Edge *prevEdge();
Node *nextNode();
Node *prevNode();
void fastestPath(Graph*, Node*, Node*);
/**@brief creates the fastest path from the first node to the second node where the edges always have the same values
 * @param graph the graph where the nodes are
 * @param startNode the node to start from
 * @param endNode the node to end at
 * @return if the path is possible, then the total cost, else -1
 */
int fastestPathOneLine(Graph*, Node*, Node*);
/**@brief prints the path created between the two given nodes along with the total cost
 */
void printPath(Node*, Node*);
