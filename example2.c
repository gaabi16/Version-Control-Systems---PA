#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

typedef struct Graph
{
    int vertices;
    int *visited;
    Node **adjacencyLists;
} Graph;

Node *createNode(int v)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Alocare de memorie esuata\n");
        exit(1);
    }
    newNode->data = v;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int vertices)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->vertices = vertices;
    graph->adjacencyLists = (Node **)malloc(vertices * sizeof(Node *));
    graph->visited = (int *)calloc(vertices, sizeof(int));
    for (int i = 0; i < vertices; i++)
    {
        graph->adjacencyLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

// adaugam noduri la finalul listei ca parcurgerile grafului sa inceapa cu left side of the descendents
// of the chosen vertex (altfel parcurgerea este de la "dreapta" la "stanga")
// pentru exemplul din word (incepand cu nodul 0):
// cu functia actuala, BFS: 0 1 3 7 2 4 6 8 5 si DFS: 0 1 2 3 4 5 6 7 8
// cu o functie care adauga noduri la inceputul listei, BFS: 0 7 3 1 8 6 4 2 5 si DFS: 0 7 8 0 3 6 4 5 1 2
void addEdge(Graph *graph, int src, int dest)
{
    Node *newNode = createNode(dest);
    if (graph->adjacencyLists[src] == NULL)
    {
        graph->adjacencyLists[src] = newNode;
    } else {
        Node *temp = graph->adjacencyLists[src];
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    newNode = createNode(src);
    if (graph->adjacencyLists[dest] == NULL)
    {
        graph->adjacencyLists[dest] = newNode;
    } else
    {
        Node *temp = graph->adjacencyLists[dest];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void insertEdges(Graph *graph, int edgeCount)
{
    int source, destination;
    printf("Introduceti %d muchii:\n", edgeCount);
    for (int i = 0; i < edgeCount; i++)
    {
        printf("Muchia %d (sursa destinatie):", i + 1);
        scanf("%d %d", &source, &destination);
        addEdge(graph, source, destination);
    }
}

void DFS(Graph *graph, int vertexNr)
{
    Node *adjList = graph->adjacencyLists[vertexNr];
    Node *temp = adjList;
    graph->visited[vertexNr] = 1;
    printf("%d ", vertexNr);

    while (temp != NULL)
    {
        int connectedVertex = temp->data;
        if (graph->visited[connectedVertex] == 0)
        {
            DFS(graph, connectedVertex);
        }
        temp = temp->next;
    }
}

int isEmpty(Node *queue)
{
    return queue == NULL;
}

void enqueue(Node **queue, int data)
{
    Node *newNode = createNode(data);
    if (*queue == NULL)
    {
        *queue = newNode;
    }
    else
    {
        Node *temp = *queue;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

int dequeue(Node **queue)
{
    if (*queue == NULL)
    {
        printf("Coada este goala\n");
        exit(1);
    }
    int data = (*queue)->data;
    Node *temp = *queue;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

void BFS(Graph *graph, int start)
{
    Node *queue = NULL;
    graph->visited[start] = 1;
    enqueue(&queue, start);

    while (!isEmpty(queue))
    {
        int current = dequeue(&queue);
        printf("%d ", current);
        Node *temp = graph->adjacencyLists[current];
        while (temp)
        {
            int adjVertex = temp->data;
            if (graph->visited[adjVertex] == 0)
            {
                graph->visited[adjVertex] = 1;
                enqueue(&queue, adjVertex);
            }
            temp = temp->next;
        }
    }
}

void wipeVisitedList(Graph *graph)
{
    for (int i = 0; i < graph->vertices; i++)
    {
        graph->visited[i] = 0;
    }
}

int main()
{
    int nrOfVertices, nrOfEdges, startingVertex;

    printf("Cate noduri are graful?");
    scanf("%d", &nrOfVertices);

    printf("Cate muchii are graful?");
    scanf("%d", &nrOfEdges);

    Graph *graph = createGraph(nrOfVertices);
    insertEdges(graph, nrOfEdges);

    printf("De unde incepem DFS?");
    scanf("%d", &startingVertex);
    printf("Parcurgere DFS:");
    DFS(graph, startingVertex);
    wipeVisitedList(graph);

    printf("\nDe unde incepem BFS?");
    scanf("%d", &startingVertex);
    printf("Parcurgere BFS:");
    BFS(graph, startingVertex);

    return 0;
}
