#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// 方向和名稱
const vector<pair<int, int>> DIRECTIONS = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
const vector<string> DIRECTION_NAMES = { "N", "NE", "E", "SE", "S", "SW", "W", "NW" };

struct Node {
    int x, y, parentX, parentY;
    string direction;
    Node(int x, int y, int px, int py, const string& dir) : x(x), y(y), parentX(px), parentY(py), direction(dir) {}
};

// 全域變數
vector<vector<int>> MAZE;
int rows, cols, ExitX, ExitY;

// 從檔案讀取迷宮
bool loadMaze(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;
    string line;
    while (getline(file, line)) {
        vector<int> row;
        for (char ch : line) row.push_back(ch - '0');
        MAZE.push_back(row);
    }
    rows = MAZE.size();
    cols = MAZE[0].size();
    ExitX = rows - 2; ExitY = cols - 2;
    return true;
}

// 打印詳細路徑和迷宮
void printPath(const vector<pair<pair<int, int>, string>>& detailedPath, const vector<pair<int, int>>& path) {
    cout << "Detailed Path:" << endl;
    for (const auto& step : detailedPath) {
        cout << "(" << step.first.first - 1 << ", " << step.first.second - 1 << ") " << step.second << endl;
    }
    cout << "Finish" << endl;

    vector<vector<char>> mazeCopy(rows, vector<char>(cols, ' '));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            mazeCopy[i][j] = (MAZE[i][j] == 1) ? '1' : '0';
        }
    }
    for (const auto& p : path) mazeCopy[p.first][p.second] = '*';

    cout << "Maze Path:" << endl;
    for (const auto& row : mazeCopy) {
        for (char cell : row) cout << cell;
        cout << endl;
    }
}

// 主程式
int main() {
    if (!loadMaze("Map.txt")) return 1;

    int startX = 1, startY = 1;
    stack<Node> stk;
	vector<vector<bool>> visited(rows, vector<bool>(cols, false))// 記錄是否走過
		vector<vector<Node>> parents(rows, vector<Node>(cols, Node(-1, -1, -1, -1, "")));// 記錄父節點

	stk.push(Node(startX, startY, -1, -1, "Start"));// 起點
	visited[startX][startY] = true;// 起點已走過

	Node endNode(-1, -1, -1, -1, "");// 終點
	while (!stk.empty()) {// DFS
		Node current = stk.top(); stk.pop();// 取出目前節點
		if (current.x == ExitX && current.y == ExitY) {// 到達終點
			endNode = current;// 紀錄終點
            break;
        }

		for (int i = 0; i < DIRECTIONS.size(); ++i) {// 嘗試八個方向
			int newX = current.x + DIRECTIONS[i].first, newY = current.y + DIRECTIONS[i].second;// 新位置
			if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && !visited[newX][newY] && MAZE[newX][newY] == 0) {// 確認新位置合法
				visited[newX][newY] = true;// 設為已走過
				stk.push(Node(newX, newY, current.x, current.y, DIRECTION_NAMES[i]));// 新位置入棧
				parents[newX][newY] = current;// 記錄父節點
            }
        }
    }

	if (endNode.x != -1 && endNode.y != -1) {// 有找到終點
		vector<pair<int, int>> path;// 簡化路徑
		vector<pair<pair<int, int>, string>> detailedPath;// 詳細路徑
		Node current = endNode;// 從終點開始
		while (current.parentX != -1 && current.parentY != -1) {// 一直往回找父節點
			path.push_back({ current.x, current.y });// 簡化路徑
			detailedPath.push_back({ {current.x + 1, current.y + 1}, current.direction });// 詳細路徑
			current = parents[current.x][current.y];// 找父節點
        }
		path.push_back({ startX, startY });// 起點
		detailedPath.push_back({ {startX + 1, startY + 1}, "Start" });// 起點
		reverse(path.begin(), path.end());// 簡化路徑反轉
		reverse(detailedPath.begin(), detailedPath.end());// 詳細路徑反轉

		printPath(detailedPath, path);// 打印路徑
    }
    else {
		cout << "No path to exit found." << endl;// 沒有找到路徑
    }

    return 0;
}
