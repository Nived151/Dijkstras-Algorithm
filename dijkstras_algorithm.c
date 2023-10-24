//GPT generated 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Define the maximum number of gates
#define MAX_GATES 100

// Structure to represent a gate in the circuit
struct Gate {
    char name[10];
    int delay;
};

// Structure to represent an edge between two gates
struct Edge {
    int from;
    int to;
    int weight;
};

// Function to calculate the shortest path using Dijkstra's algorithm
void dijkstra(int n, struct Edge edges[], int start, int end) {
    int distance[MAX_GATES];
    int visited[MAX_GATES];
    int predecessor[MAX_GATES];

    // Initialize distance and predecessor arrays
    for (int i = 0; i < n; i++) {
        distance[i] = INT_MAX;
        visited[i] = 0;
        predecessor[i] = -1;
    }

    distance[start] = 0;

    // Dijkstra's algorithm
    for (int i = 0; i < n - 1; i++) {
        int minDistance = INT_MAX;
        int u;

        // Find the vertex with the minimum distance
        for (int v = 0; v < n; v++) {
            if (!visited[v] && distance[v] < minDistance) {
                minDistance = distance[v];
                u = v;
            }
        }

        visited[u] = 1;

        // Update the distance of adjacent vertices
        for (int j = 0; j < n; j++) {
            if (!visited[j] && edges[u * n + j].weight != -1 &&
                distance[u] + edges[u * n + j].weight < distance[j]) {
                distance[j] = distance[u] + edges[u * n + j].weight;
                predecessor[j] = u;
            }
        }
    }

    // Print the shortest path and delay
    if (distance[end] != INT_MAX) {
        int current = end;
        printf("Shortest path from %s to %s:\n", edges[start * n + end].name, edges[end * n + start].name);
        printf("%s", edges[end * n + start].name);

        while (predecessor[current] != -1) {
            printf(" <- %s", edges[predecessor[current] * n + current].name);
            current = predecessor[current];
        }

        printf("\nTotal Delay: %d units\n", distance[end]);
    } else {
        printf("No path found from %s to %s.\n", edges[start * n + end].name, edges[end * n + start].name);
    }
}

int main() {
    int num_gates = 8;
    struct Gate gates[MAX_GATES];
    struct Edge edges[MAX_GATES * MAX_GATES];

    // Initialize gates with names and delays
    strcpy(gates[0].name, "G1gat");
    gates[0].delay = 0;
    strcpy(gates[1].name, "G2gat");
    gates[1].delay = 0;
    strcpy(gates[2].name, "G3gat");
    gates[2].delay = 0;
    strcpy(gates[3].name, "G6gat");
    gates[3].delay = 0;
    strcpy(gates[4].name, "G7gat");
    gates[4].delay = 0;
    strcpy(gates[5].name, "G10gat");
    gates[5].delay = 3;  // Delay of G10gat
    strcpy(gates[6].name, "G19gat");
    gates[6].delay = 3;  // Delay of G19gat
    strcpy(gates[7].name, "G22gat");
    gates[7].delay = 3;  // Delay of G22gat

    // Initialize edges with weights (delays between gates)
    for (int i = 0; i < num_gates; i++) {
        for (int j = 0; j < num_gates; j++) {
            if (i != j) {
                edges[i * num_gates + j].from = i;
                edges[i * num_gates + j].to = j;
                edges[i * num_gates + j].weight = gates[j].delay;
                strcpy(edges[i * num_gates + j].name, gates[i].name);
            } else {
                edges[i * num_gates + j].from = i;
                edges[i * num_gates + j].to = j;
                edges[i * num_gates + j].weight = -1;  // Self-loop with weight -1
                strcpy(edges[i * num_gates + j].name, gates[i].name);
            }
        }
    }

    // Find the shortest path from input to output
    dijkstra(num_gates, edges, 0, 7);  // Change 0 and 7 based on your inputs

    return 0;
}
