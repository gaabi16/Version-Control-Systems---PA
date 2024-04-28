#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct Graph {
    int vertexCount;
    int *visited;
    Node **adjacencyList;
} Graph;

typedef struct Stack {
    int top;
    int capacity;
    int *elements;
} Stack;

Node *createNode(int value)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

void addEdgeToGraph(Graph *graph, int source, int destination)
{
    Node *newNode = createNode(destination);
    newNode->next = graph->adjacencyList[source];
    graph->adjacencyList[source] = newNode;

    newNode = createNode(source);
    newNode->next = graph->adjacencyList[destination];
    graph->adjacencyList[destination] = newNode;
}

Graph *createGraph(int vertexCount)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (graph == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    graph->vertexCount = vertexCount;
    graph->adjacencyList = (Node **)malloc(vertexCount * sizeof(Node *));
    graph->visited = (int *)calloc(vertexCount, sizeof(int));

    for (int i = 0; i < vertexCount; i++)
    {
        graph->adjacencyList[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

Stack *createStack(int capacity)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    stack->capacity = capacity;
    stack->top = -1;
    stack->elements = (int *)malloc(capacity * sizeof(int));
    if (stack->elements == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }
    return stack;
}

void push(Stack *stack, int item)
{
    if (stack->top == stack->capacity - 1)
    {
        printf("Stack overflow\n");
        exit(1);
    }
    stack->elements[++stack->top] = item;
}

void DFS(Graph *graph, Stack *stack, int vertex)
{
    Node *adjacencyList = graph->adjacencyList[vertex];
    Node *auxiliaryAdjacencyList = adjacencyList;
    graph->visited[vertex] = 1;
    push(stack, vertex);

    while (auxiliaryAdjacencyList != NULL)
    {
        int connectedVertex = auxiliaryAdjacencyList->data;
        if (graph->visited[connectedVertex] == 0)
            DFS(graph, stack, connectedVertex);
        auxiliaryAdjacencyList = auxiliaryAdjacencyList->next;
    }
}

void insertEdges(Graph *graph, int edgeCount)
{
    int source, destination;
    printf("Enter %d edges:\n", edgeCount);
    for (int i = 0; i < edgeCount; i++)
    {
        printf("Edge %d (source destination):", i + 1);
        scanf("%d %d", &source, &destination);
        addEdgeToGraph(graph, source, destination);
    }
}

void resetVisited(Graph *graph)
{
    for (int i = 0; i < graph->vertexCount; i++)
    {
        graph->visited[i] = 0;
    }
}

bool areConnected(Graph *graph, int source, int destination, Stack *stack)
{
    DFS(graph, stack, source);
    bool reachable = graph->visited[destination];
    resetVisited(graph);
    return reachable;
}

int main()
{
    int vertexCount, edgeCount, source, destination;

    printf("Enter the number of vertices in the graph:");
    scanf("%d", &vertexCount);

    printf("Enter the number of edges in the graph:");
    scanf("%d", &edgeCount);

    Graph *graph = createGraph(vertexCount);
    Stack *stack = createStack(2 * vertexCount);

    insertEdges(graph, edgeCount);

    printf("Enter 2 vertices to check if they are connected:");
    scanf("%d %d", &source, &destination);

    if (areConnected(graph, source, destination, stack))
        printf("Yes, there is a path between restaurant %d and restaurant %d.\n", source, destination);
    else
        printf("No, there is no path between restaurant %d and restaurant %d.\n", source, destination);

    return 0;
}
