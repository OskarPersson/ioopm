typedef struct node Node;
typedef struct edge Edge;

Node* mkNode(void*);
Edge* mkEdge(Node*, Node*, void*);
void* getValue(Node*);
Node* getEdgeFirst(Edge*);
Node* getEdgeSecond(Edge*);
void* getEdgeValue(Edge*);
