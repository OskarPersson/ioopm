typedef struct graph Graph;
typedef struct node Node;
typedef struct edge Edge;

Graph* mkGraph(Edge**, int);
void rmGraph(Graph*);
int graphSize(Graph*);
Edge** graphEdges(Graph*);
Node* mkNode(void*);
Edge* mkEdge(Node*, Node*, int, void*);
void* getValue(Node*);
void setNodeValue(Node*, void*);
Node* getEdgeFirst(Edge*);
Node* getEdgeSecond(Edge*);
void setEdgeFirst(Edge*, Node*);
void setEdgeSecond(Edge*, Node*);
int getEdgeCost(Edge*);
void* getEdgeValue(Edge*);
void fastestPath(Graph*, Node*, Node*);
