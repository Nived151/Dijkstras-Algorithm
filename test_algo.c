#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX

// This structure represents a directed graph using adjacency list representation
struct Graph {
    int V; // Number of vertices

    // In a weighted graph, we need to store vertex
    // and weight pair for every edge
    struct Pair {
        int v;
        int w;
    }** adj;
};

// Function to create a new graph with V vertices
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->adj = (struct Pair**)malloc(V * sizeof(struct Pair*));

    for (int i = 0; i < V; i++) {
        graph->adj[i] = NULL;
    }

    return graph;
}

// Function to add an edge to the graph
void addEdge(struct Graph* graph, int u, int v, int w) {
    struct Pair* newNode = (struct Pair*)malloc(sizeof(struct Pair));
    newNode->v = v;
    newNode->w = w;
    newNode->next = graph->adj[u];
    graph->adj[u] = newNode;

    newNode = (struct Pair*)malloc(sizeof(struct Pair));
    newNode->v = u;
    newNode->w = w;
    newNode->next = graph->adj[v];
    graph->adj[v] = newNode;
}

// Function to print the shortest path from source to all other vertices
void shortestPath(struct Graph* graph, int src) {
    int V = graph->V;
    int* dist = (int*)malloc(V * sizeof(int));
    int* visited = (int*)malloc(V * sizeof(int));

    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u, minDist = INF;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && dist[v] < minDist) {
                u = v;
                minDist = dist[v];
            }
        }

        visited[u] = 1;

        for (struct Pair* p = graph->adj[u]; p != NULL; p = p->next) {
            int v = p->v;
            int weight = p->w;

            if (!visited[v] && dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }

    printf("Vertex Distance from Source\n");
    for (int i = 0; i < V; i++) {
        printf("%d \t\t %d\n", i, dist[i]);
    }

    free(dist);
    free(visited);
}

// Driver's code
int main() {
    // Create the graph given in the example
    int V = 9;
    struct Graph* graph = createGraph(V);

    // Add edges to the graph
    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 7, 8);
    addEdge(graph, 1, 2, 8);
    addEdge(graph, 1, 7, 11);
    addEdge(graph, 2, 3, 7);
    addEdge(graph, 2, 8, 2);
    addEdge(graph, 2, 5, 4);
    addEdge(graph, 3, 4, 9);
    addEdge(graph, 3, 5, 14);
    addEdge(graph, 4, 5, 10);
    addEdge(graph, 5, 6, 2);
    addEdge(graph, 6, 7, 1);
    addEdge(graph, 6, 8, 6);
    addEdge(graph, 7, 8, 7);

    // Find and print the shortest paths
    shortestPath(graph, 0);

    return 0;
}
