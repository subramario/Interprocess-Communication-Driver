#include <iostream>
#include <list>
#include <vector>
#include <regex>
#include <iterator>
#include <string>
#include <stack>
#include <list>

using namespace std; 

// Creates an adjacency list (ie. array of arrays)
void add_edge(vector<int> adj_list[], int u, int v){
    adj_list[u].push_back(v); // add v into list u
    adj_list[v].push_back(u); // add u into list v
}

// BFS algorithm to find the shortest path between 2 nodes
void BFS_Shortest(int start, int end, int V, vector<int> adj_list[]){ 

    vector<int> line; // Used to simulate a queue data structure 
    bool visited[V]; // Keeps track of all nodes visited
    int prev[V]; // Links path from source node to end node

    for(int i = 0; i<V; i++){
        visited[i] = false; // "false" initialization indicates no nodes have been traversed yet
        prev[i] = -1; // prev[i] is the direct predeccesor of i -> ie. value node is predecessor of indice node
    }

    visited[start] = true; // Initialize start node as visited 
    line.push_back(start); 

    while (!line.empty()){
        int node = line.front(); // Peak at front of line --> store value in variable "node"
        line.erase(line.begin()); // Simulates dequeueing

        if (adj_list){
            
            // Loops through all connected vertices for node in adjacency list
            for(unsigned int i = 0; i < adj_list[node].size(); i++){

                // If node has been visited, this code will not execute --> saves time
                if (visited[adj_list[node][i]] == false){ 
                    
                    visited[adj_list[node][i]] = true;
                    prev[adj_list[node][i]] = node;
                    line.push_back(adj_list[node][i]);

                    if (adj_list[node][i] == end){
                        stack <int> shortest_path; // Use stack data structure to recreate the shortest path from start node to end node
                        int temp = end;
                        shortest_path.push(end);
                        while(prev[temp] != -1){
                            shortest_path.push(prev[temp]);
                            temp = prev[temp];
                        }

                        // Popping off the stack reproduces the shortest path from start node to end node
                        while(!shortest_path.empty()){ 
                            int i = shortest_path.top();
                            if (i == end)
                                cout << i << endl;
                            else
                                cout << i << "-";
                            shortest_path.pop();
                        }   
                              
                        return;
                    }
                }
            }
        }
    }
    cout << "Error: Vertices not connected - shortest path cannot be displayed." << endl;
    return;
}


int main(int argc, char* argv[]){

    smatch m;
    string input;
    int a,b,c,V = 1;
    vector <int> *adj_list = NULL;

    // Matches command input to appropriate loop
    regex vert_rx("^[V]"); 
    regex edge_rx("^[E]");
    regex short_rx("^[S]");

    // Extract relevant info from input
    const regex numbers("([0-9]+)+");
    const regex coordinates("([0-9]+[,][0-9]+)+");

     
    while (!cin.eof()){ // eof allows program to exit gracefully
        getline(cin, input);

        // Executes this block of code if the number of vertices is specifed through the "V" command
        if (regex_search(input,m,vert_rx)){
            regex_search(input, m, numbers);

            string temp = m[0]; // Stores the match object in string "temp"
            a = stoi(temp); // Converts the string to an integer

            // Error case --> user specifies a graph with zero vertices
            if (a == 0) 
                cout << "Error: Graph cannot have zero vertices!" << endl;
            else
                V = a;
        }

        // Executes this block of code if the edge list is specifed through the "E" command
        if (regex_search(input,m,edge_rx)){
            adj_list = new vector<int>[V];

            // Creates a match object, "m", with all edges parsed from user input using regex "coordinates"
            while (regex_search(input,m,coordinates)){
            
                string s = m.str(0); // Parses one edge in the match object at a time
                
                // Extracts vertices from the edge coordinate
                string delimiter_1 = ",";
                string delimiter_2 = ">";
                string token_1 = s.substr(0, s.find(delimiter_1));
                string token_2 = s.substr(s.find(",")+1, s.find(delimiter_2));
                b = stoi(token_1);
                c = stoi(token_2);


                // Error case --> edge specification includes the same vertice twice
                if (b == c){ 
                    cout << "Error: Cannot have an edge between a vertice and itself!" << endl;
                    break;
                }

                // Error case --> edge specification includes a vertex higher than the total number of vertices in the graph
                if (b >= V || c >= V){ 
                    cout << "Error: Cannot have an edge between non-existant vertices." << endl;
                    break;
                }

                bool duplication = false;
                bool inverse_duplication = false;

                // Error case --> duplicate edges
                for (unsigned int i = 0; i < adj_list[b].size(); i++){ 
                    if (adj_list[b][i] == c){
                        cout << "Error: System does not allow duplicate edges." << endl;
                        duplication = true;
                        break;
                    }
                }

                if (duplication == true)
                    break;

                // Error case --> inverse duplicate edges
                for (unsigned int i = 0; i < adj_list[c].size(); i++){ 
                    if (adj_list[c][i] == b){
                        cout << "Error: System does not allow duplicate edges." << endl;
                        inverse_duplication = true;
                        break;
                    }
                }

                if (inverse_duplication == true)
                    break;
                
                // If the edge specification is valid, add it to the adjacency list
                if (duplication == false && inverse_duplication == false) 
                    add_edge(adj_list, b ,c);

                // Used to iterate through remaining edges in the match object
                input = m.suffix(); 

            }

        }

        // Executes this block of code if the edge list is specifed through the "S" command
        if (regex_search(input,m,short_rx)){

            regex_search(input,m,numbers);
            int start = stoi(m[0]);

            input = m.suffix();
            regex_search(input,m,numbers);
            int end = stoi(m[0]);

            // Error case --> user tries to specify shortest distance between a vertex and itself
            if (start == end){ 
                cout << "Error: Cannot have self-loop in an undirected graph!" << endl;
                continue;
            }

            // Error case --> user tries to specify shortest distance to a vertex that does not exist
            else if (start >= V || end >= V){ 
                cout << "Error: Vertices do not exist - shortest path does therefore, not exist." << endl;
                continue;
            }

            else
                BFS_Shortest(start, end, V, adj_list);
        }
    }


   return 0;
} 