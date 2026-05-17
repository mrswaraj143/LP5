#include <iostream>
#include <queue>
#include <stack>
#include <omp.h>
#include <vector>

using namespace std;

class Graph{

    int v;
    vector<vector<int>> adj;

    public:
    Graph(int v){
        this->v = v;
        adj.resize(v);
    }

    void addEdge(int a, int b){
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    void parallelBfs(int start){
        queue<int> q;
        vector<bool> visited(v,false);

        visited[start] = true;
        q.push(start);

        cout<< "parallel Bfs using openmp"<<endl;

        while(!q.empty()){
            int node = q.front();
            q.pop();
            cout << node <<"->";

            #pragma omp parallel for
            for(int i=0;i<adj[node].size();i++){
                int neighbours = adj[node][i];

                if(!visited[neighbours]){
                    #pragma omp critical
                    if(!visited[neighbours]){
                        visited[neighbours] = true;
                        q.push(neighbours);
                    }
                }
            }
        }
        cout <<endl;
    }

    void parallelDfs(int start){
        vector<bool> visited(v,false);
        stack<int> st;

        visited[start] = true;
        st.push(start);

        cout<< "parallel dfs using openmp"<<endl;

        while(!st.empty()){
            int node = st.top();
            st.pop();
            cout<<node<<"->";

            #pragma omp parallel for
            for(int i=0;i<adj[node].size();i++){
                int neighbour = adj[node][i];

                if(!visited[neighbour]){
                    #pragma omp critical
                    if(!visited[neighbour]){
                        visited[neighbour] = true;
                        st.push(neighbour);
                    }
                }
            }
        }
    }

};

int main(){
    int vertices, edges;
    cout << "Enter number of vertices and edges: ";
    cin >> vertices >> edges;

    Graph g(vertices);

    cout << "Enter " << edges << " edges (u v):" << endl;
    for (int i = 0; i < edges; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start_node;
    cout << "Enter the starting node for BFS and DFS: ";
    cin >> start_node;

    g.parallelBfs(start_node);

    g.parallelDfs(start_node);
    cout << endl;

    return 0;
}

/*

Enter number of vertices and edges: 7 11
 Enter 11 edges (u v): 
 0 1 
 0 3 
 1 2 
 1 3 
 1 6 
 1 5 
 2 4 
 2 3 
 2 5 
 4 6 
 3 4 
Enter the starting node for BFS and DFS: 0 
parallel Bfs using openmp 
0->1->3->2->6->5->4-> 
parallel dfs using openmp 
0->3->4->6->2->5->1->

*/