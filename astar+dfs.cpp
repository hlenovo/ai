#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <unordered_map>

using namespace std;

struct Node {
    int x, y;
    double g, h;
    Node* parent;

    Node(int x, int y, double g = 0, double h = 0, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), parent(parent) {}

    double f() const {
        return g + h;
    }

    bool operator>(const Node& other) const {
        return f() > other.f();
    }
};

double heuristic(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

vector<Node*> get_neighbors(Node* node, const vector<vector<int>>& grid) {
    vector<Node*> neighbors;
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; ++i) {
        int nx = node->x + dx[i];
        int ny = node->y + dy[i];

        if (nx >= 0 && nx < grid.size() && ny >= 0 && ny < grid[0].size() && grid[nx][ny] == 0) {
            neighbors.push_back(new Node(nx, ny, node->g + 1, heuristic(nx, ny, node->x, node->y), node));
        }
    }

    return neighbors;
}

vector<Node*> reconstruct_path(Node* node) {
    vector<Node*> path;
    while (node) {
        path.push_back(node);
        node = node->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<Node*> a_star_dfs(const vector<vector<int>>& grid, Node* start, Node* goal) {
    stack<Node*> open_set;
    unordered_map<int, Node*> all_nodes;

    start->h = heuristic(start->x, start->y, goal->x, goal->y);
    open_set.push(start);
    all_nodes[start->x * grid[0].size() + start->y] = start;

    while (!open_set.empty()) {
        Node* current = open_set.top();
        open_set.pop();

        if (current->x == goal->x && current->y == goal->y) {
            return reconstruct_path(current);
        }

        for (Node* neighbor : get_neighbors(current, grid)) {
            double tentative_g = current->g + 1;

            if (all_nodes.find(neighbor->x * grid[0].size() + neighbor->y) == all_nodes.end() || tentative_g < neighbor->g) {
                neighbor->g = tentative_g;
                neighbor->h = heuristic(neighbor->x, neighbor->y, goal->x, goal->y);
                neighbor->parent = current;
                open_set.push(neighbor);
                all_nodes[neighbor->x * grid[0].size() + neighbor->y] = neighbor;
            }
        }
    }

    return vector<Node*>();
}

int main() {
    vector<vector<int>> grid = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };

    Node* start = new Node(0, 0);
    Node* goal = new Node(4, 4);

    vector<Node*> path = a_star_dfs(grid, start, goal);

    if (!path.empty()) {
        cout << "Path found:" << endl;
        for (Node* node : path) {
            cout << "(" << node->x << ", " << node->y << ")" << endl;
        }
    } else {
        cout << "No path found." << endl;
    }

    // Giải phóng bộ nhớ
    for (Node* node : path) {
        delete node;
    }
    delete start;
    delete goal;

    return 0;
}
