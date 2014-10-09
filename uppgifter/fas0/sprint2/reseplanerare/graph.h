typedef struct node Node;
typedef struct edge Edge;

Node* mkNode(void*);
Edge* mkEdge(Node*, Node*, int, void*);
void* getValue(Node*);
Node* getEdgeFirst(Edge*);
Node* getEdgeSecond(Edge*);
int getEdgeCost(Edge*);
void* getEdgeValue(Edge*);
Edge** nodeEdges(Edge**, int, Node*);
Edge** fastestPath(Edge**, int, Node*, Node*);
