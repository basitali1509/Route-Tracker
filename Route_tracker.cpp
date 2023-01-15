#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

// Class Node to represent a node in a map
class Node
{
public:
    string name;      // name of place
    double latitude;  // latitude coordinate
    double longitude; // longitude coordinate

    double g; // cost from start to current node
    double h; // estimated cost from current node to end
    double f; // total cost (f = g + h )

    double traffic; // traffic update on the node to neighbor node

    Node *parent; // pointer to parent node

    Node(string name, double lat, double lon, double traffic)
    {
        this->name = name;
        longitude = lon;
        latitude = lat;

        f = 0, g = 0, h = 0;

        this->traffic = traffic;

        parent = nullptr;
    }
};

// Class to represent the map
class Map
{
public:
    unordered_map<string, Node *> nodes; // dictionary of nodes in the map

    // Adds a node in the map
    void add_Node(Node *node)
    {
        nodes[node->name] = node; // similar to like nodes["karachi"] = (its long and lat along with f,g and h value)
    }

    // Returns the distance between two nodes
    double get_Distance(Node *m, Node *n)
    {
        double X = m->longitude - n->longitude;
        double Y = m->latitude - n->latitude;

        double total = sqrt(X * X + Y * Y);

        // traffic condition of node being traveled to
        total *= (1 + n->traffic);

        return total;
    }

    // Returns the list of neighboring nodes for a given node
    vector<Node *> get_Neighbors(Node *node)
    {
        double MAX_DISTANCE = 0.016; // Maximum distance for a node to be considered a neighbor
        vector<Node *> neighbors;

        // Iterate over all the nodes in the map
        for (auto &node_pair : nodes)
        {

            Node *neighbor = node_pair.second;
            // Calculate the distance between the current node and the neighbor
            double distance = get_Distance(node, neighbor);

            // If the distance is within the desired range, add the neighbor to the list
            if (distance < MAX_DISTANCE && neighbor->traffic <= 0.5)
            { // exclude neighbors with traffic condition above 0.5
                neighbors.push_back(neighbor);
            }
        }
        return neighbors;
    }
};

// A* algorithm to find the shortest path between two nodes in the map
vector<Node *> A_Star(Map *m, Node *start, Node *end)
{
    // Initialize the open and closed sets
    vector<Node *> open;
    unordered_map<string, Node *> closed;
    vector<Node *> path;

    // Add the start node to the open set
    open.push_back(start);
    start->g = 0;
    start->h = m->get_Distance(start, end);
    start->f = start->g + start->h;

    while (!open.empty())
    {
        // Get the node with the lowest f value from the open set
        int min_i = 0;
        for (int i = 1; i < open.size(); i++)
        {
            if (open[i]->f < open[min_i]->f)
            {
                min_i = i;
            }
        }
        Node *current = open[min_i];
        open.erase(open.begin() + min_i);

        // Add the current node to the closed set
        closed[current->name] = current;

        // If the current node is the end node, we have found the shortest path
        if (current == end)
        {
            // Retrace the path from the end node to the start node
            while (current != start)
            {
                path.push_back(current);
                current = current->parent;
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            break;
        }

        // Get the neighboring nodes of the current node
        vector<Node *> neighbors = m->get_Neighbors(current);
        for (Node *neighbor : neighbors)
        {
            // Skip the neighbor if it is already in the closed set
            if (closed.count(neighbor->name) != 0)
            {
                continue;
            }

            // Calculate the new g value for the neighbor
            double g_score = current->g + m->get_Distance(current, neighbor);

            // Check if the neighbor is in the open set
            bool neighbor_in_open = find(open.begin(), open.end(), neighbor) != open.end();

            // Update the neighbor if it is not in the open set, or if the new g value is lower than the current g value
            if (!neighbor_in_open || g_score < neighbor->g)
            {
                neighbor->g = g_score;
                neighbor->h = m->get_Distance(neighbor, end);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;
                if (!neighbor_in_open)
                {
                    open.push_back(neighbor);
                }
            }
        }
    }
    return path;
}
int main()
{

    // Create the map
    Map map;

    // Add nodes (locations) to the map

    // Node("Place Name", latitude, longitude, traffic)
    //{No traffic if value is less than 0.5 else that node will be ignored program will show the alternate route}
    
    map.add_Node(new Node("Nagan Chowrangi", 24.96504, 67.06686, 0.0));
    map.add_Node(new Node("Shafiq Mor", 24.95725, 67.07620, 0.0));
    map.add_Node(new Node("LuckyOne Mall", 24.94521, 67.08630, 0.0));
    map.add_Node(new Node("Gulshan Chowrangi", 24.93438, 67.09156, 0.0));
    map.add_Node(new Node("NIPA", 24.92081, 67.09913, 0.0));
    map.add_Node(new Node("NED University", 24.91276, 67.11323, 0.0));
    map.add_Node(new Node("Maskan Chowrangi", 24.9340, 67.1053, 0.5));
    map.add_Node(new Node("UIT University", 24.92788, 67.10871, 0.0));
    map.add_Node(new Node("Safora", 24.92016, 67.12297, 0.0));
    map.add_Node(new Node("Johar", 24.90566, 67.10178, 0.0));
    map.add_Node(new Node("Millennium Mall", 24.89927, 67.11300, 0.0));
    map.add_Node(new Node("Drig Road", 24.89044, 67.12560, 0.0));
    map.add_Node(new Node("Kalaboard", 24.88219, 67.11272, 0.0));


    // User current location
    string start_name, end_name;
    cout << "Enter the name of your current location: ";
    getline(cin, start_name);
    cout << endl;

    // User desired destination
    cout << "Enter the name of your desired destination: ";
    getline(cin, end_name);

    Node *start = map.nodes[start_name];
    Node *end = map.nodes[end_name];
    vector<Node *> path = A_Star(&map, start, end);

    cout << endl
         << endl;
    cout << "Shortest path from " << start_name << " to " << end_name << ": " << endl
         << endl;

    // Print the shortest path from start (current location) till end (destination)
    for (int i = 0; i < path.size() - 1; i++)
    {
        cout << path[i]->name << " -> ";
    }
    cout << path.back()->name << endl;
    cout << endl
         << endl;

    return 0;
}
