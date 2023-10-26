#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIGNALS 100
#define INFINITY 1000000

// Structure to represent gates
typedef struct {
    char name[20];
    int fanout;
    int delay;
    int visited;
} Gate;

// Function to find the index of a gate by name
int findGateIndex(Gate gates[], char name[], int numGates) {
    for (int i = 0; i < numGates; i++) {
        if (strcmp(gates[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to implement Dijkstra's algorithm
int dijkstra(Gate gates[], int numGates, char start[], char end[]) {
    int distances[MAX_SIGNALS];
    int prev[MAX_SIGNALS];

    int startIndex = findGateIndex(gates, start, numGates);
    int endIndex = findGateIndex(gates, end, numGates);

    if (startIndex == -1 || endIndex == -1) {
        return -1; // Signal not found
    }

    for (int i = 0; i < numGates; i++) {
        distances[i] = INFINITY;
        prev[i] = -1;
        gates[i].visited = 0;
    }

    distances[startIndex] = 0;

    for (int count = 0; count < numGates; count++) {
        int minDistance = INFINITY;
        int minIndex = -1;

        for (int i = 0; i < numGates; i++) {
            if (!gates[i].visited && distances[i] < minDistance) {
                minDistance = distances[i];
                minIndex = i;
            }
        }

        if (minIndex == -1) {
            break; // No reachable nodes left
        }

        gates[minIndex].visited = 1;

        for (int i = 0; i < numGates; i++) {
            int delay = gates[minIndex].fanout * gates[i].delay;

            if (!gates[i].visited && delay > 0 && distances[minIndex] + delay < distances[i]) {
                distances[i] = distances[minIndex] + delay;
                prev[i] = minIndex;
            }
        }
    }

    if (distances[endIndex] >= INFINITY) {
        return -2; // No path found
    }

    return distances[endIndex];
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <bench_file> <input_signal> <output_signal>\n", argv[0]);
        return 1;
    }

    char *bench = argv[1];
    char *inputSignal = argv[2];
    char *outputSignal = argv[3];

    // Read the circuit description from the bench file
    FILE *file = fopen(bench, "r");
    if (file == NULL) {
        printf("Wrong file name\n");
        return 1;
    }

    Gate gates[MAX_SIGNALS];
    int numGates = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, " \n");
        if (strcmp(token, "INPUT") == 0) {
            token = strtok(NULL, " \n");
            strcpy(gates[numGates].name, token);
            gates[numGates].fanout = 0;
            gates[numGates].delay = 0;
            numGates++;
        } else if (strcmp(token, "OUTPUT") == 0) {
            token = strtok(NULL, " \n");
            strcpy(gates[numGates].name, token);
            gates[numGates].fanout = 0;
            gates[numGates].delay = 0;
            numGates++;
        } else if (strcmp(token, "=") == 0) {
            token = strtok(NULL, " =\n");
            int index = findGateIndex(gates, token, numGates);
            if (index != -1) {
                gates[index].fanout++;
                token = strtok(NULL, " =\n");
                gates[index].delay = atoi(token);
            }
        }
    }
    fclose(file);

    int delay = dijkstra(gates, numGates, inputSignal, outputSignal);

    if (delay == -1) {
        printf("Signal not found in file\n");
    } else if (delay == -2) {
        printf("No path found\n");
    } else {
        printf("Shortest delay from %s to %s: %d units\n", inputSignal, outputSignal, delay);
    }

    return 0;
}