#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GATE_NAME_LEN 10
#define MAX_NUM_GATES 100
#define INF 999999

typedef struct {
    char name[MAX_GATE_NAME_LEN];
    int delay;
} Gate;

int num_gates = 0;
Gate gates[MAX_NUM_GATES];

typedef struct {
    char from[MAX_GATE_NAME_LEN];
    char to[MAX_GATE_NAME_LEN];
    int fanout;
} Connection;

int num_connections = 0;
Connection connections[MAX_NUM_GATES];

// Function to add a gate to the list
void addGate(const char name[MAX_GATE_NAME_LEN], int delay) {
    if (num_gates < MAX_NUM_GATES) {
        strcpy(gates[num_gates].name, name);
        gates[num_gates].delay = delay;
        num_gates++;
    }
}

// Function to add a connection to the list
void addConnection(const char from[MAX_GATE_NAME_LEN], const char to[MAX_GATE_NAME_LEN], int fanout) {
    if (num_connections < MAX_NUM_GATES) {
        strcpy(connections[num_connections].from, from);
        strcpy(connections[num_connections].to, to);
        connections[num_connections].fanout = fanout;
        num_connections++;
    }
}

// Function to find the gate index by name
int findGateIndex(const char name[MAX_GATE_NAME_LEN]) {
    for (int i = 0; i < num_gates; i++) {
        if (strcmp(gates[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Dijkstra's algorithm to find the shortest path
int dijkstra(int start, int end) {
    int dist[MAX_NUM_GATES];
    int visited[MAX_NUM_GATES];

    for (int i = 0; i < num_gates; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }

    dist[start] = 0;

    for (int count = 0; count < num_gates - 1; count++) {
        int minDist = INF;
        int u = -1;

        for (int v = 0; v < num_gates; v++) {
            if (!visited[v] && dist[v] < minDist) {
                minDist = dist[v];
                u = v;
            }
        }

        visited[u] = 1;

        for (int v = 0; v < num_gates; v++) {
            if (!visited[v] && connections[u].fanout > 0) {
                int alt = dist[u] + (connections[u].fanout * gates[v].delay);
                if (alt < dist[v]) {
                    dist[v] = alt;
                }
            }
        }
    }

    return dist[end];
}

int main(int argc, char** argv) {
    // Define the circuit gates and delays
    addGate("G1gat", 1);
    addGate("G2gat", 1);
    addGate("G3gat", 1);
    addGate("G6gat", 1);
    addGate("G7gat", 1);
    addGate("G10gat", 1);
    addGate("G11gat", 1);
    addGate("G16gat", 1);
    addGate("G19gat", 1);
    addGate("G22gat", 1);
    addGate("G23gat", 1);

    // Define the connections and fanouts
    addConnection("G1gat", "G10gat", 1);
    addConnection("G3gat", "G10gat", 1);
    addConnection("G3gat", "G11gat", 1);
    addConnection("G6gat", "G11gat", 1);
    addConnection("G11gat", "G16gat", 1);
    addConnection("G7gat", "G19gat", 1);
    addConnection("G10gat", "G22gat", 1);
    addConnection("G16gat", "G2gat", 1);
    addConnection("G16gat", "G23gat", 1);
    addConnection("G19gat", "G23gat", 1);

    // Find the shortest path in terms of delay from a user-provided input to output
    char startGate[MAX_GATE_NAME_LEN];
    char endGate[MAX_GATE_NAME_LEN];

    //printf("Enter the name of the starting gate: ");
    //scanf("%s", startGate);
    //printf("Enter the name of the ending gate: ");
    //scanf("%s", endGate);

    strcpy(startGate,argv[1]);
    strcpy(endGate,argv[2]);

    int start = findGateIndex(startGate);
    int end = findGateIndex(endGate);

    if (start != -1 && end != -1) {
        int shortestDelay = dijkstra(start, end);
        printf("Shortest delay from %s to %s is %d units.\n", startGate, endGate, shortestDelay);
    } else {
        printf("Invalid gate names.\n");
    }

    return 0;
}
