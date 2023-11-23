/**
 * Group ID -
 * Member 1 Name -
 * Member 1 BITS ID -
 * Member 2 Name -
 * Member 2 BITS ID -
 * Member 3 Name -
 * Member 3 BITS ID -
 */

// ---------------------------DO NOT MODIFY (Begin)------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Graph
{
    int n;                // Size of the graph
    int **adj;            // Adjacency matrix
    char **station_names; // Array of station names
} Graph;

/**
 * This function has been pre-filled. It reads the input testcase
 * and creates the Graph structure from it.
 */
Graph *create_graph(char input_file_path[])
{
    FILE *f = fopen(input_file_path, "r");
    Graph *g = (Graph *)malloc(sizeof(Graph));
    // Size of graph
    fscanf(f, "%d", &(g->n));
    // Names of stations
    g->station_names = (char **)malloc(g->n * sizeof(char *));
    for (int i = 0; i < g->n; i++)
    {
        g->station_names[i] = (char *)malloc(100 * sizeof(char));
        fscanf(f, "%s", g->station_names[i]);
    }
    // Adjacency matrix
    g->adj = (int **)malloc(g->n * sizeof(int *));
    for (int i = 0; i < g->n; i++)
    {
        g->adj[i] = calloc(g->n, sizeof(int));
    }
    int edges;
    fscanf(f, "%d", &edges);
    for (int i = 0; i < edges; i++)
    {
        int x, y;
        fscanf(f, "%d%d", &x, &y);
        g->adj[x][y] = 1;
        g->adj[y][x] = 1;
    }
    fclose(f);
    return g;
}
// ---------------------------DO NOT MODIFY (End)------------------------------

/**
 * Q.1
 * Return the number of junctions.
 */
int find_junctions(Graph *g)
{
    int num_j = 0;
    for (int i = 0; i < g->n; i++)
    {
        int d = 0;
        for (int j = 0; j < g->n; j++)
        {
            if (g->adj[i][j])
            {
                d++;
            }
        }
        if (d >= 4)
        {
            num_j++;
        }
    }
    return num_j;
}

/**
 * Q.2
 * Return true if the tour specified in the question is possible for this
 * graph, else return false.
 * Parameter SAME_STATION - If it is true, then question 2(a) must be solved.
 * If false, then question 2(b) must be solved.
 */
bool sheldons_tour(Graph *g, bool SAME_STATION)
{
    int num_odd_deg = 0;

    for (int i = 0; i < g->n; i++)
    {
        int d = 0;
        for (int j = 0; j < g->n; j++)
        {
            if (g->adj[i][j])
            {
                d++;
            }
        }
        if (d % 2 != 0)
        {
            num_odd_deg++;
        }
    }
    if (num_odd_deg == 0 && SAME_STATION)
    {
        return true;
    }
    else if (num_odd_deg == 2 && !SAME_STATION)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Q.3
 * Return closure, an n * n matrix filled with 0s and 1s as required.
 */
int **warshall(Graph *g)
{
    // Do not modify or delete pre-filled code!
    int **closure = (int **)malloc(g->n * sizeof(int *));
    for (int i = 0; i < g->n; i++)
    {
        closure[i] = calloc(g->n, sizeof(int));
    }

    // Code goes here
    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
        {
            if (g->adj[i][j])
            {
                closure[i][j] = 1;
            }
        }
    }

    for (int k = 0; k < g->n; k++)
    {
        for (int i = 0; i < g->n; i++)
        {
            for (int j = 0; j < g->n; j++)
            {
                if (closure[i][k] && closure[k][j])
                {
                    closure[i][j] = 1;
                }
            }
        }
    }

    return closure; // Do not modify
}

/**
 * Q.3
 * Return the number of impossible pairs.
 */
int find_impossible_pairs(Graph *g)
{
    int **closure = warshall(g); // Do not modify

    int z_count = 0;
    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
        {
            if (!closure[i][j])
            {
                z_count++;
            }
        }
    }
    return z_count / 2;
}

/**
 * Q.4
 * Return the number of vital train tracks.
 */
int find_vital_train_tracks(Graph *g)
{
    int count = 0;
    for (int i = 0; i < g->n; i++)
    {
        for (int j = i + 1; j < g->n; j++)
        {
            if (g->adj[i][j] == 1)
            {
                g->adj[i][j] = 0;
                g->adj[j][i] = 0;

                if (!warshall(g)[i][j])
                {
                    // If not connected, the edge is vital
                    count++;
                }

                // Restore the edge
                g->adj[i][j] = 1;
                g->adj[j][i] = 1;
            }
        }
    }
    return count;
}

/**
 * Q.5
 * Return upgrades, an array of size n.
 * upgrades[i] should be 0 if railway station i gets a restaurant
 * upgrades[i] should be 1 if railway station i gets a maintenance depot
 * If it is not possible to upgrade, then return "-1" in the entire array
 */
int *upgrade_railway_stations(Graph *g)
{
    int *upgrades = calloc(g->n, sizeof(int));
    upgrades[0] = 0; // Assign restaurant to first station
    upgrades[1] = 1; // Assign maintenance depot to second station
    for (int i = 2; i < g->n; i++)
    {
        bool restaurant_connected = false;
        bool depot_connected = false;
        for (int j = 0; j < i; j++)
        {
            if (g->adj[i][j] == 1)
            {
                if (upgrades[j] == 0)
                {
                    restaurant_connected = true;
                }
                else if (upgrades[j] == 1)
                {
                    depot_connected = true;
                }
            }
        }
        if (restaurant_connected && !depot_connected)
        {
            upgrades[i] = 1; // Assign maintenance depot
        }
        else if (!restaurant_connected && depot_connected)
        {
            upgrades[i] = 0; // Assign restaurant
        }
        else
        {
            // Not possible to upgrade
            for (int j = 0; j < g->n; j++)
            {
                upgrades[j] = -1;
            }
            break;
        }
    }
    return upgrades;
}

/**
 * Q.6
 * Return distance between X and Y
 * city_x is the index of X, city_y is the index of Y
 */
int distance(Graph *g, int city_x, int city_y)
{
    if (city_x == city_y)
    {
        return 0;
    }
    int queue[g->n];
    int front = -1, rear = -1;
    int visited[g->n];
    int distanceArray[g->n];
    for (int i = 0; i < g->n; i++)
    {
        visited[i] = 0;
        distanceArray[i] = -1;
    }
    queue[++rear] = city_x;
    visited[city_x] = 1;
    distanceArray[city_x] = 0;

    while (front != rear)
    {
        int currentCity = queue[++front];

        for (int i = 0; i < g->n; i++)
        {
            if (g->adj[currentCity][i] == 1 && !visited[i])
            {
                queue[++rear] = i;
                visited[i] = 1;
                distanceArray[i] = distanceArray[currentCity] + 1;
                if (i == city_y)
                {
                    return distanceArray[i];
                }
            }
        }
    }
    return -1;
}

/**
 * Q.7
 * Return the index of any one possible railway capital in the network
 */
int railway_capital(Graph *g)
{
    int minSum = 1000;
    int rC = 0;

    for (int i = 0; i < g->n; i++)
    {
        int sum = 0;
        for (int j = 0; j < g->n; j++)
        {
            if (distance(g, i, j) == -1)
            {
                sum = sum + 100;
                break;
            }
            sum = sum + distance(g, i, j);
        }
        if (sum < minSum)
        {
            minSum = sum;
            rC = i;
        }
    }
    return rC;
}

/**
 * Helper function for Q.8
 */
bool maharaja_express_tour(Graph *g, int source, int current_city, int previous_city, int *visited)
{
    visited[current_city] = 1;
    for (int i = 0; i < g->n; i++)
    {
        if (g->adj[current_city][i] == 1 && visited[i] == 0)
        {
            if (maharaja_express_tour(g, source, i, current_city, visited))
            {
                return true;
            }
        }
        else if (g->adj[current_city][i] == 1 && visited[i] == 1 && i == source && i != previous_city)
        {
            return true;
        }
    }
    visited[current_city] = 0;
    return false;
}

/**
 * Q.8
 * Return true if it is possible to have a Maharaja Express tour
 * starting from source city, else return false.
 */
bool maharaja_express(Graph *g, int source)
{
    int *visited = (int *)malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; i++)
    {
        visited[i] = 0;
    }
    // Hint: Call the helper function and pass the visited array created here.
    bool tour_possible = maharaja_express_tour(g, source, source, -1, visited);
    free(visited);
    return tour_possible;
}

int main()
{
    char input_file_path[100] = "../resources/testcases/testcase_4.txt"; // Can be modified
    Graph *g = create_graph(input_file_path);                            // Do not modify

    // Code goes here
    printf("Number of junction = %d\n\n", find_junctions(g));
    printf("Sheldon's tour (ending in same city as start) = ");
    printf(sheldons_tour(g, true) ? "POSSIBLE\n" : "IMPOSSIBLE\n");
    printf("Sheldon's tour (ending in different city as start) = ");
    printf(sheldons_tour(g, false) ? "POSSIBLE\n" : "IMPOSSIBLE\n");
    printf("\nTransitive closure = \n");
    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
        {
            printf("%d ", warshall(g)[i][j]);
        }
        printf("\n");
    }
    printf("\nImpossible to travel city pairs = %d\n", find_impossible_pairs(g));
    printf("\nNumber of vital train tracks = %d\n", find_vital_train_tracks(g));

    printf("\nRailway Station Upgrades: \n");
    for (int i = 0; i < g->n; i++)
    {
        if (upgrade_railway_stations(g)[i] == -1)
        {
            printf("\nRailway Station Upgrades = NOT POSSIBLE\n");
            break;
        }
        printf("%s = %s\n", g->station_names[i], upgrade_railway_stations(g)[i] ? "Maintenance Depot" : "Restaurant");
    }

    printf("\nDistance between %s and %s = %d\n", g->station_names[0], g->station_names[4], distance(g, 0, 4));
    printf("\nRailway Capital = %s\n\n", g->station_names[railway_capital(g)]);
    for (int i = 0; i < g->n; i++)
    {
        printf("Maharaja Express tour starting from %s = %s\n", g->station_names[i], maharaja_express(g, i) ? "POSSIBLE" : "IMPOSSIBLE");
    }

    return 0;
}