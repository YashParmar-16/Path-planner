
/*
* AstroTinker Bot (AB): Task 1B Path Planner
*
* This program computes the valid path from the start point to the end point.
* Make sure you don't change anything outside the "Add your code here" section.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __linux__ // for host pc

    #include <stdio.h>

    void _put_byte(char c) { putchar(c); }

    void _put_str(char *str) {
        while (*str) {
            _put_byte(*str++);
        }
    }

    void print_output(uint8_t num) {
        if (num == 0) {
            putchar('0'); // if the number is 0, directly print '0'
            _put_byte('\n');
            return;
        }

        if (num < 0) {
            putchar('-'); // print the negative sign for negative numbers
            num = -num;   // make the number positive for easier processing
        }

        // convert the integer to a string
        char buffer[20]; // assuming a 32-bit integer, the maximum number of digits is 10 (plus sign and null terminator)
        uint8_t index = 0;

        while (num > 0) {
            buffer[index++] = '0' + num % 10; // convert the last digit to its character representation
            num /= 10;                        // move to the next digit
        }

        // print the characters in reverse order (from right to left)
        while (index > 0) { putchar(buffer[--index]); }
        _put_byte('\n');
    }

    void _put_value(uint8_t val) { print_output(val); }

#else  // for the test device

    void _put_value(uint8_t val) { }
    void _put_str(char *str) { }

#endif


// main function
int main(int argc, char const *argv[]) {

    #ifdef __linux__

        const uint8_t START_POINT   = atoi(argv[1]);
        const uint8_t END_POINT     = atoi(argv[2]);
        uint8_t CPU_DONE            = 0;

    #else

        #define START_POINT         (* (volatile uint8_t * ) 0x20000000)
        #define END_POINT           (* (volatile uint8_t * ) 0x20000001)
        #define CPU_DONE            (* (volatile uint8_t * ) 0x20000003)

    #endif

    // array to store the planned path
    uint8_t path_planned[32];

    // index to keep track of the path_planned array
    uint8_t idx = 0;

    // instead of using printf() function for debugging,
    // use the below function calls to print a number, string or a newline

    // for newline: _put_byte('\n');
    // for string:  _put_str("your string here");
    // for number:  _put_value(your_number_here);

    // Examples:
    // _put_value(START_POINT);
    // _put_value(END_POINT);
    // _put_str("Hello World!");
    // _put_byte('\n');

    // ############# Add your code here #############
    #define INT_MAX 2147483647
        // Define the maximum number of nodes in the graph
#define MAX_NODES 30

// Define the adjacency list for the graph
int graph[MAX_NODES][MAX_NODES] = {
    {1},     // Node 0
    {0, 2, 29},  // Node 1
    {1, 3, 8},   // Node 2
    {2, 4},     // Node 3
    {3, 5, 6},  // Node 4
    {4},     // Node 5
    {4, 7},     // Node 6
    {6, 8},     // Node 7
    {2, 7, 9, 12},  // Node 8
    {8, 10, 11},    // Node 9
    {9},     // Node 10
    {9},     // Node 11
    {8, 13},    // Node 12
    {12, 14},   // Node 13
    {13, 15, 16},  // Node 14
    {14},    // Node 15
    {14, 17, 18},  // Node 16
    {16},    // Node 17
    {16, 19},   // Node 18
    {18, 20},   // Node 19
    {19, 21, 24, 29},  // Node 20
    {20, 22, 23},     // Node 21
    {21},    // Node 22
    {21},    // Node 23
    {20, 25},  // Node 24
    {24, 26},  // Node 25
    {25, 27, 28},  // Node 26
    {26},    // Node 27
    {26, 29},  // Node 28
    {1, 20, 28}  // Node 29
};

// Function to find the node with the minimum distance
int minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < MAX_NODES; v++) {
        if (!sptSet[v] && dist[v] < min) {
            min = dist[v];
            min_index = v;
        }
    }

    return min_index;
}

void printPath(int parent[], int destination, uint8_t path_planned[], uint8_t *idx) {
    if (parent[destination] == -1)
        return;

    printPath(parent, parent[destination], path_planned, idx);
    path_planned[(*idx)++] = destination;
}

// Function to implement Dijkstra's algorithm and store the path in path_planned

void dijkstra(int src, int dest, uint8_t path_planned[], uint8_t *idx) {
    int dist[MAX_NODES];  // Array to store the shortest distances
    bool sptSet[MAX_NODES];  // Array to keep track of visited nodes
    int parent[MAX_NODES];  // Array to store the parent node of each node in the shortest path
    *idx = 0;
    for (int i = 0; i < MAX_NODES; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
        parent[i] = -1;
    }

    dist[src] = 0;

    for (int count = 0; count < MAX_NODES - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;

        for (int v = 0; v < MAX_NODES; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] + 1 < dist[v]) {
                dist[v] = dist[u] + 1;
                parent[v] = u;
            }
        }
    }

    // Store the shortest path in path_planned
    printPath(parent, dest, path_planned, idx);
}
void storePath(int parent[], int end, uint8_t path_planned[], uint8_t* idx) {
    if (parent[end] == -1) {
        path_planned[(*idx)++] = end;
        return;
    }

    storePath(parent, parent[end], path_planned, idx);
    path_planned[(*idx)++] = end;
}


// Function to perform Dijkstra's algorithm and store the path
void findAndStoreShortestPath(int start, int end, uint8_t path_planned[], uint8_t *idx) {
    // Check if the start and end points are within a valid range
    if (start < 0 || start >= MAX_NODES || end < 0 || end >= MAX_NODES) {
        _put_str("Invalid start or end point.\n");
    } else {
        // Find and store the shortest path
        dijkstra(start, end, path_planned, idx);
    }
    int dist[MAX_NODES];
    bool sptSet[MAX_NODES];
    int parent[MAX_NODES];

    // Initialize idx to 0
    *idx = 0;

    // The rest of the code remains the same
    storePath(parent, end, path_planned, idx);
}

// Example usage:
int main(int argc, char const *argv[]) {
    uint8_t path_planned[32];
    uint8_t idx = 0;

    #ifdef __linux__

        const uint8_t START_POINT   = atoi(argv[1]);
        const uint8_t END_POINT     = atoi(argv[2]);
        uint8_t CPU_DONE            = 0;

    #else
        // Replace these values with your specific start and end points.
        int start_point = 0;
        int end_point = 8;
        findAndStoreShortestPath(start_point, end_point, path_planned, &idx);
    #endif

    // Print the planned path
    _put_str("######### Planned Path #########\n");
    for (int i = 0; i < idx; ++i) {
        _put_value(path_planned[i]);
    }
    _put_str("################################\n");

    return 0;
}


    // ##############################################

    #ifdef __linux__    // for host pc

        _put_str("######### Planned Path #########\n");
        for (int i = 0; i < idx; ++i) {
            _put_value(path_planned[i]);
        }
        _put_str("################################\n");

    #endif

    return 0;
}
