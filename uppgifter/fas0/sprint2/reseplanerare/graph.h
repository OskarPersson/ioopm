typedef struct graph Graph;
typedef struct node Node;
typedef struct edge Edge;

Graph *mkGraph(Edge**, int);
void rmGraph(Graph*);
void *getGraphValue(Graph*);
void setGraphValue(Graph*, void*);
int graphSize(Graph*);
Edge **graphEdges(Graph*);
Node *mkNode(void*);
void rmNode(Node*, Graph*);
Edge *mkEdge(Node*, Node*, int, void*);
void rmEdge(Edge*, Graph*);
void *getNodeValue(Node*);
void setNodeValue(Node*, void*);
Node *getEdgeFirst(Edge*);
Node *getEdgeSecond(Edge*);
void setEdgeFirst(Edge*, Node*);
void setEdgeSecond(Edge*, Node*);
int getEdgeCost(Edge*);
void *getEdgeValue(Edge*);
void setEdgeValue(Edge*, void*);
void fastestPath(Graph*, Node*, Node*);
