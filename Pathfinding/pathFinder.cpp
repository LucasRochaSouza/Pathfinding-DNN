#include "pathFinder.h"

std::ostream& operator<<(std::ostream& os, const point& pos) {
    os << "( " << pos.x << ", " << pos.y << " )";
    return os;
}

bool operator==(const Node n1, const Node n2)
{
    if (n1.position.x == n2.position.x &&
        n1.position.y == n2.position.y) {
        return true;
    }
    else {
        return false;
    }
}

int distance(Node nodeA, Node nodeB) {
    double x = nodeA.position.x - nodeB.position.x; //calculating number to square in next step
    double y = nodeA.position.y - nodeB.position.y;
    double dist;

    dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
    dist = sqrt(dist);

    return (int)dist;
}

int manhattan_distance(Node nodeA, Node nodeB)
{
    int dstX = abs(nodeA.position.x - nodeB.position.x);
    int dstY = abs(nodeA.position.y - nodeB.position.y);

    if (dstX > dstY)
        return 14 * dstY + 10 * (dstX - dstY);
    return 14 * dstX + 10 * (dstY - dstX);
}

vector<point> find_path(Node startNode, Node targetNode, Grid grid, point top, point bot) {
    Heap openSet(grid.Width * grid.Height);

    openSet.add(&startNode);

    int count = 0;

    vector<point> path;

    while (openSet.currentItemCount > 0)
    {
        Node* currentNode = openSet.removeFirst();
        currentNode->visited = true;
        count++;

        if (*currentNode == targetNode)
        {
            Node current = *currentNode;
            while (true) {
                path.push_back(current.position);
                if (current.getParent() == nullptr) {
                    return path;
                }
                else {
                    current = current.getParent();
                }
            }
        }

        std::vector<Node*> vetor = grid.GetNeighbours(*currentNode);

        for (Node* neighbour : vetor)
        {
            if (neighbour->position.x < top.x || neighbour->position.x > bot.x ||
                neighbour->position.y < top.y || neighbour->position.y > bot.y) {
                continue;
            }

            if (neighbour->visited == true) {
                continue;
            }

            int newMovementCostToNeighbour = currentNode->g_cost() + manhattan_distance(*currentNode, *neighbour);
            if (newMovementCostToNeighbour < neighbour->g_cost() || !openSet.contains(*neighbour))
            {
                neighbour->set_g(newMovementCostToNeighbour);
                neighbour->set_h(manhattan_distance(*neighbour, *currentNode));

                neighbour->set_parent(currentNode);

                if (!openSet.contains(*neighbour))
                    openSet.add(neighbour);
            }
        }
    }
    return vector<point>(0);
}

vector<point> find_path(Node startNode, Node targetNode, Grid grid) {
    for(int x = 0; x < grid.Width; x++)
        for (int y = 0; y < grid.Height; y++) {
            grid.grid[x][y]->visited = false;
            grid.grid[x][y]->set_g(0);
            grid.grid[x][y]->HeapIndex = 0;
        }
    Heap openSet(grid.Width * grid.Height);

    openSet.add(&startNode);

    int count = 0;

    vector<point> path;

    while (openSet.currentItemCount > 0)
    {
        Node* currentNode = openSet.removeFirst();
        currentNode->visited = true;
        count++;

        if (*currentNode == targetNode)
        {
            Node current = *currentNode;
            while (true) {
                path.push_back(current.position);
                if (current.getParent() == nullptr) {
                    return path;
                }
                else {
                    current = current.getParent();
                }
            }
        }

        std::vector<Node*> vetor = grid.GetNeighbours(*currentNode);

        for (Node* neighbour : vetor)
        {
            if (neighbour->visited == true) {
                continue;
            }

            int newMovementCostToNeighbour = currentNode->g_cost();

            //int newMovementCostToNeighbour = currentNode->g_cost() + manhattan_distance(*currentNode, *neighbour);
            if (newMovementCostToNeighbour < neighbour->g_cost() || !openSet.contains(*neighbour))
            {
                neighbour->set_g(newMovementCostToNeighbour + 1);
                neighbour->set_h(manhattan_distance(*neighbour, *currentNode));

                neighbour->set_parent(currentNode);

                if (!openSet.contains(*neighbour))
                    openSet.add(neighbour);
            }
        }
    }
    return vector<point>(0);
}

vector<point> find_path_(Node startNode, Node targetNode, Grid grid, DistanceMatrix dm) {
    for (int x = 0; x < grid.Width; x++)
        for (int y = 0; y < grid.Height; y++) {
            grid.grid[x][y]->visited = false;
            grid.grid[x][y]->set_g(0);
            grid.grid[x][y]->HeapIndex = 0;
        }
    Heap openSet(grid.Width * grid.Height);

    openSet.add(&startNode);

    int count = 0;

    vector<point> path;

    while (openSet.currentItemCount > 0)
    {
        Node* currentNode = openSet.removeFirst();
        currentNode->visited = true;
        count++;

        if (*currentNode == targetNode)
        {
            Node current = *currentNode;
            while (true) {
                path.push_back(current.position);
                if (current.getParent() == nullptr) {
                    return path;
                }
                else {
                    current = current.getParent();
                }
            }
        }

        std::vector<Node*> vetor = grid.GetNeighbours(*currentNode);

        for (Node* neighbour : vetor)
        {
            if (neighbour->visited == true) {
                continue;
            }

            int newMovementCostToNeighbour = currentNode->g_cost();

            //int newMovementCostToNeighbour = currentNode->g_cost() + manhattan_distance(*currentNode, *neighbour);
            if (newMovementCostToNeighbour < neighbour->g_cost() || !openSet.contains(*neighbour))
            {
                neighbour->set_g(newMovementCostToNeighbour + 1);

                neighbour->set_h(dm.distances(neighbour->position.x, neighbour->position.y,
                    targetNode.position.x, targetNode.position.y));

                neighbour->set_parent(currentNode);

                if (!openSet.contains(*neighbour))
                    openSet.add(neighbour);
            }
        }
    }
    return vector<point>(0);
}


void BFS(int source, std::vector<int>& dist, graph_adj_list<int> adj_list) {
    
    queue<int> queue;
    int size = adj_list.get_size(); 

    dist = vector<int>(size);

    vector<bool> visited(size);


    for (int i = 0; i < size; i++) {
        visited[i] = false;
        dist[i] = INT_MAX;
    }

    visited[source] = true;
    dist[source] = 0;
    queue.push(source);

    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();
        for (int i = 0; i < adj_list[u].size(); i++) {
            if (visited[adj_list[u][i]] == false) {
                visited[adj_list[u][i]] = true;
                dist[adj_list[u][i]] = dist[u] + 1;
                queue.push(adj_list[u][i]);
            }
        }
    }
}
