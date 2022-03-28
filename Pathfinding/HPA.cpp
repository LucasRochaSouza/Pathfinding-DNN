#include "HPA.h"

int pos_2(int x, int y, int width) {
    if (x < 0 || y < 0) {
        return -1;
    }
    return x + width * y;
}

int pos_2(point p, int width) {
    return pos_2(p.x, p.y, width);
}

void HPA::add_edge_graph(Node n1, Node n2, int weight) {
    global_graph.addEdge(n1.position, n2.position, weight);
}

void HPA::compareCluster(Cluster &c1, Cluster &c2) {
    if (c1.getLeftTop().x == c2.getLeftTop().x) {

        // Sul

        if (c1.getRightBotton().y == c2.getLeftTop().y || c1.getRightBotton().y + 1 == c2.getLeftTop().y ||
            c1.getRightBotton().y == c2.getLeftTop().y + 1) {
            for (int i = c1.getLeftTop().x; i < c1.getLeftTop().x + c1.getWidth(); i++) {
                Node src(i, c2.getLeftTop().y - 1);
                Node dst(i, c2.getLeftTop().y);

                if (global_grid.get(src.position.x, src.position.y) && global_grid.get(dst.position.x, dst.position.y)) {
                    createInterClusterEdge(src.position, dst.position);
                    c1.addBorder(src);
                    c2.addBorder(dst);
                }
            }
        }
    }

    else if (c1.getLeftTop().y == c2.getLeftTop().y || c1.getLeftTop().y + 1 == c2.getLeftTop().y ||
        c1.getLeftTop().y == c2.getLeftTop().y + 1) {

        // Leste
        if (c1.getRightBotton().x == c2.getLeftTop().x || c1.getRightBotton().x + 1 == c2.getLeftTop().x ||
            c1.getRightBotton().x == c2.getLeftTop().x + 1) {
            for (int i = c2.getLeftTop().y; i < c2.getLeftTop().y + c2.getHeight(); i++) {
                Node src(c2.getLeftTop().x - 1, i);
                Node dst(c2.getLeftTop().x, i);

                if (global_grid.get(src.position.x, src.position.y) && global_grid.get(dst.position.x, dst.position.y)) {
                    createInterClusterEdge(src.position, dst.position);
                    c1.addBorder(src);
                    c2.addBorder(dst);
                }
            }
        }
    }
}

void HPA::buildClusters(int horizontal, int vertical, BinaryImage source)
{
    //global_grid = source;
    //astar_grid = Grid(global_grid);
    //bfs_graph = global_grid;

    vector<Cluster> clusters;

    int width = source.width;

    int height = source.height;

    int x_loc = 0;
    int y_loc = 0;

    float clusterSize = sqrt((source.width * source.height/(horizontal*vertical)));

    int c_size = floor(clusterSize);

    for (int i = 0; i < horizontal; i++) {
        for (int j = 0; j < vertical; j++) {
            
            point leftTop = point(j * (c_size), i * (c_size));

            int rightX = leftTop.x + c_size;
            int rightY = leftTop.y + c_size;

            if (j == vertical - 1) {
                rightX = width;
            }
            if (i == horizontal - 1) {
                rightY = height;
            }

            point rightBotton = point(rightX, rightY);

            BinaryImage clusterSplit = splitImage(source, leftTop.x, rightX, leftTop.y, rightY);

            Cluster tempCluster = Cluster(clusterSplit, leftTop, rightBotton, clusters.size());

            clusters.push_back(tempCluster);
        }
    }

    for (int i = 0; i < horizontal * vertical; i++) {
        for (int j = i + 1; j < horizontal * vertical; j++) {
            compareCluster(clusters[i], clusters[j]);
        }
    }

    this->clusters = clusters;
}

void HPA::createInterClusterEdge(int x1, int y1, int x2, int y2)
{
    point pos1(x1, y1);
    point pos2(x2, y2);

    createInterClusterEdge(pos1, pos2);
}

void HPA::createInterClusterEdge(point p1, point p2)
{
    add_edge_graph(p1, p2, 1);
    add_edge_graph(p2, p1, 1);
}

bool HPA::connectNodes(Cluster c, BinaryImage map) {
    vector<int> dist;
    Graph2D g(c.getGrid());

    int width = c.getGrid().width;
    int height = c.getGrid().height;

    for (int i = 0; i < c.nodes.size(); i++) {
        Node n1 = Node(c.nodes[i].position);
        g.BFS(n1.position.x - c.getLeftTop().x,
            n1.position.y - c.getLeftTop().y, dist);

        for (int j = i + 1; j < c.nodes.size(); j++) {
            Node n2 = Node(c.nodes[j].position);
            int tempX = n2.position.x - c.getLeftTop().x;
            int tempY = n2.position.y - c.getLeftTop().y;

            point temp = point(tempX, tempY);

            int arr = pos_2(temp, width);
            if (dist[arr] > 0 && dist[arr] != INT_MAX) {
                add_edge_graph(n1.position, n2.position, dist[arr]);
                add_edge_graph(n2.position, n1.position, dist[arr]);
            }
        }
    }
    return true;
}

void HPA::connectToBorder_DHPA(point pos, Cluster c, Graph_t graph) {
    for (AbstractNode node : c.nodes) {
        int tempX = pos.x + c.getLeftTop().x;
        int tempY = pos.y + c.getLeftTop().y;
        point corrected(tempX, tempY);
        int distance_to_point = node.distance_data[pos_2(corrected, global_grid.width)];
        if (distance_to_point != INT_MAX) {
            graph.addEdge(pos, node.position, distance_to_point);
            graph.addEdge(node.position, pos, distance_to_point);
        }
    }
}


void HPA::connectToBorder(point pos, Cluster c, Graph_t graph)
{
    Graph2D g(c.getGrid());

    vector<int> dist;

    int width = c.getGrid().width;
    int height = c.getGrid().height;

    int x1;
    int y1;

    x1 = pos.x - c.getLeftTop().x;
    y1 = pos.y - c.getLeftTop().y;

    if (x1 < 0 || y1 < 0) {
        return;
    }

    g.BFS(x1, y1, dist);

    for (int i = 0; i < c.nodes.size(); i++) {
        Node n2 = Node(c.nodes[i].position);
        int tempX = n2.position.x - c.getLeftTop().x;
        int tempY = n2.position.y - c.getLeftTop().y;

        int arr = pos_2(point(tempX, tempY), width);

        if (arr > width * height)
            continue;

        if (dist[arr] > 0 && dist[arr] != INT_MAX) {
            graph.addEdge(pos, n2.position, dist[arr]);
            graph.addEdge(n2.position, pos, dist[arr]);
        }
    }
}

bool HPA::insert_start_finish(point src, point dst, Graph_t g) {
    int start = -1;
    int finish = -1;

    for (int i = 0; i < clusters.size(); i++) {
        if (clusters[i].contains(src)) {
            start = i;
        }

        if (clusters[i].contains(dst)) {
            finish = i;
        }
    }

    if (start == finish) {
        return true;
    }
    else {
        connectToBorder(src, clusters[start], g);
        connectToBorder(dst, clusters[finish], g);
        return false;
    }
}

void HPA::abstractMaze(BinaryImage source, int vertical, int horizontal)
{
    global_grid = source;
    global_graph = Graph_t(global_grid.width * global_grid.height, global_grid.width);
    astar_grid = Grid(source);
    astar_map.set(source);
    buildClusters(vertical, horizontal, source);

    for (int i = 0; i < clusters.size(); i++) {
        //Graph2D g(clusters[i].getGrid());

        //clusters[i].dm = DistanceMatrix(g);

        connectNodes(clusters[i], global_grid);
    }

    //insert_start_finish(point(2, 2), point(73, 73));

    //global_graph.save_graph("file.dot");

    //auto abstract_path = global_graph.shortestPath(point(2, 2), point(73, 73));
    //auto path = global_graph.shortestPath(clusters[0].nodes[0].position, clusters[3].nodes[0].position);
}

Cluster HPA::findCluster(point pos1, point pos2) {
    for (auto cluster : clusters) {
        if (cluster.contains(pos1)) {
            if (cluster.contains(pos2)) {
                return cluster;
            }
        }
    }
    return clusters[-1];
}

vector<point> HPA::abstract_search(int src, int dst)
{
    return std::vector<point>();
    /*
    graph_adj_list<AbstractNode> abs_graph;
    // Create a priority queue to store vertices that 
    // are being preprocessed. This is weird syntax in C++. 
    // Refer below link for details of this syntax 
    priority_queue< iPair, vector <iPair>, greater<iPair> > pq;

    // Create a vector for distances and initialize all 
    // distances as infinite (INF) 
    vector<int> dist(abs_graph.get_size(), INT_MAX);
    vector<int> parent(abs_graph.get_size(), -1);
    vector<point> path;
    // Insert source itself in priority queue and initialize 
    // its distance as 0. 
    pq.push(make_pair(0, src));
    dist[src] = 0;

    // Looping till priority queue becomes empty (or all
    //  distances are not finalized) 
    while (!pq.empty())
    {
        // The first vertex in pair is the minimum distance 
        // vertex, extract it from priority queue. 
        // vertex label is stored in second of pair (it 
        // has to be done this way to keep the vertices 
        // sorted distance (distance must be first item 
        // in pair) 
        int u = pq.top().second;
        pq.pop();

        // 'i' is used to get all adjacent vertices of a vertex 
        list< pair<int, int> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            // Get vertex label and weight of current adjacent 
            // of u. 
            int v = (*i).first;
            int weight = (*i).second;

            //  If there is shorted path to v through u. 
            if (dist[v] > dist[u] + weight)
            {
                // Updating distance of v 
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
                parent[v] = u;
                if (v == dst) {
                    int currentVertex = v;
                    while (true) {
                        int x = currentVertex % global_grid.width;
                        int y = currentVertex / global_grid.width;

                        path.push_back(point(x, y));

                        if (parent[currentVertex] == -1) {

                            return path;
                        }
                        else {
                            currentVertex = parent[currentVertex];
                        }
                    }
                }
            }
        }
    }
    return path;
    */
}

int distance(point p1, point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

Cluster HPA::findCluster(point pos)
{
    for (auto cluster : clusters) {
        if (cluster.contains(pos)) {
            return cluster;
        }
    }
    return clusters[-1];
}

void HPA::calculate_path_job(point src, point dst, vector<point> &path) {
    if (distance(src, dst) < 2) {
        path.push_back(src);
    }
    Cluster c = findCluster(src, dst);

    A_star_fast foo;
    foo.set(global_grid);

    int nodes = 0;

    int x1 = src.x;
    int y1 = src.y;
    int x2 = dst.x;
    int y2 = dst.y;

    point test1(x1, y1);
    point test2(x2, y2);

    //auto temp = find_path(Node(x2, y2), Node(x1, y1), g, c.getLeftTop(), c.getRightBotton());

    auto temp = foo.compute(x1, y1, x2, y2);

    if (temp.size() == 0) {
        test_info.expandedNodes = -1;
    }
    //auto temp = Astar_2(grid, paths[i], paths[i + 1], nodes);
    test_info.expandedNodes += nodes;

    for (auto p : temp) {
        path.push_back(p);
    }
}

std::vector<point> HPA::HPA_Star_MT(point src, point dst) {
    vector<point> abstract_path;
    vector<point> final_path;

    test_info.executionTime = 0;
    test_info.distance = 0;
    test_info.expandedNodes = 0;

    //insert_start_finish(src, dst);

    //point start_border = select_best_node(src, findCluster(src));

    //point end_border = select_best_node(dst, findCluster(dst));

    abstract_path = global_graph.shortestPath(dst, src);


    vector<thread> thread_list;
    vector<vector<point>> path;

    thread_list.resize(abstract_path.size());
    path.resize(abstract_path.size());

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    for (int i = 0; i < abstract_path.size() - 1; i++) {
        thread_list[i] = thread(&HPA::calculate_path_job, this, abstract_path[i], abstract_path[i + 1], ref(path[i]));
    }

    for (int i = 0; i < abstract_path.size() - 1; i++) {
        thread_list[i].join();
    }

    for (vector<point> p : path) {
        for (point point : p) {
            final_path.push_back(point);
        }
    }


    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() * 1000 << endl;

    return final_path;
}

std::vector<point> HPA::HPA_Star(point src, point dst)
{
    vector<point> abstract_path;
    vector<point> final_path;

    test_info.executionTime = 0;
    test_info.distance = 0;
    test_info.expandedNodes = 0;

    //astar_map.set(global_grid);

    Graph_t temp_graph = global_graph;

    if (insert_start_finish(src, dst, global_graph)) {
        test_info.expandedNodes = -1;
        return final_path;
    }

    abstract_path = global_graph.shortestPath(dst, src);

    if (abstract_path.size() < 1) {
        test_info.expandedNodes = -1;
        return final_path;
    }

    final_path.push_back(abstract_path[0]);
    
    float total_time = 0;
   
    int nodes = 0;
    int clusters_traveled = 0;
    for (int i = 0; i < abstract_path.size() - 1; i++) {
        if (distance(abstract_path[i], abstract_path[i + 1]) < 2) {
            final_path.push_back(abstract_path[i]);
            clusters_traveled++;
            test_info.clusters_traveled++;
            continue;
        }
        int local_nodes;
        int x1 = abstract_path[i].x;
        int y1 = abstract_path[i].y;
        int x2 = abstract_path[i + 1].x;
        int y2 = abstract_path[i + 1].y;

        auto concrete_path = find_path(Node(x2, y2), Node(x1, y1), astar_grid);

        if (concrete_path.size() == 0) {
            test_info.expandedNodes = 0;
            return std::vector<point>(0);
        }


        for (auto p : concrete_path) {
            final_path.push_back(p);
        }
    }

    test_info.executionTime =  total_time;
    test_info.distance = final_path.size();
    test_info.expandedNodes = nodes;
    global_graph.removeVertex(src);
    global_graph.removeVertex(dst);

    return final_path;
}

