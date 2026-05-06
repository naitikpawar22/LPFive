#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // Add edge (undirected graph)
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // ================= PARALLEL BFS =================
    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS Traversal: ";

        while (!q.empty()) {
            int levelSize = q.size();

            // Process current level in parallel
            #pragma omp parallel for
            for (int i = 0; i < levelSize; i++) {

                int node;

                // Critical section for queue access
                #pragma omp critical
                {
                    if (!q.empty()) {
                        node = q.front();
                        q.pop();
                        cout << node << " ";
                    }
                }

                // Explore neighbors
                for (int neighbor : adj[node]) {
                    if (!visited[neighbor]) {

                        #pragma omp critical
                        {
                            if (!visited[neighbor]) {
                                visited[neighbor] = true;
                                q.push(neighbor);
                            }
                        }
                    }
                }
            }
        }
        cout << endl;
    }

    // ================= PARALLEL DFS =================
    void parallelDFSUtil(int node, vector<bool>& visited) {

        #pragma omp critical
        {
            cout << node << " ";
        }

        visited[node] = true;

        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); i++) {
            int neighbor = adj[node][i];

            if (!visited[neighbor]) {
                #pragma omp task
                {
                    if (!visited[neighbor]) {
                        parallelDFSUtil(neighbor, visited);
                    }
                }
            }
        }
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);

        cout << "Parallel DFS Traversal: ";

        #pragma omp parallel
        {
            #pragma omp single
            {
                parallelDFSUtil(start, visited);
            }
        }

        cout << endl;
    }
};

// ================= MAIN FUNCTION =================
int main() {
    int V = 6;
    Graph g(V);

    // Example graph (undirected)
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    cout << "Graph Traversal using OpenMP\n";

    g.parallelBFS(0);
    g.parallelDFS(0);

    return 0;
}