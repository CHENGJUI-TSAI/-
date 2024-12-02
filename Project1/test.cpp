#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// ��V�M�W��
const vector<pair<int, int>> DIRECTIONS = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
const vector<string> DIRECTION_NAMES = { "N", "NE", "E", "SE", "S", "SW", "W", "NW" };

struct Node {
    int x, y, parentX, parentY;
    string direction;
    Node(int x, int y, int px, int py, const string& dir) : x(x), y(y), parentX(px), parentY(py), direction(dir) {}
};

// �����ܼ�
vector<vector<int>> MAZE;
int rows, cols, ExitX, ExitY;

// �q�ɮ�Ū���g�c
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

// ���L�ԲӸ��|�M�g�c
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

// �D�{��
int main() {
    if (!loadMaze("Map.txt")) return 1;

    int startX = 1, startY = 1;
    stack<Node> stk;
	vector<vector<bool>> visited(rows, vector<bool>(cols, false))// �O���O�_���L
		vector<vector<Node>> parents(rows, vector<Node>(cols, Node(-1, -1, -1, -1, "")));// �O�����`�I

	stk.push(Node(startX, startY, -1, -1, "Start"));// �_�I
	visited[startX][startY] = true;// �_�I�w���L

	Node endNode(-1, -1, -1, -1, "");// ���I
	while (!stk.empty()) {// DFS
		Node current = stk.top(); stk.pop();// ���X�ثe�`�I
		if (current.x == ExitX && current.y == ExitY) {// ��F���I
			endNode = current;// �������I
            break;
        }

		for (int i = 0; i < DIRECTIONS.size(); ++i) {// ���դK�Ӥ�V
			int newX = current.x + DIRECTIONS[i].first, newY = current.y + DIRECTIONS[i].second;// �s��m
			if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && !visited[newX][newY] && MAZE[newX][newY] == 0) {// �T�{�s��m�X�k
				visited[newX][newY] = true;// �]���w���L
				stk.push(Node(newX, newY, current.x, current.y, DIRECTION_NAMES[i]));// �s��m�J��
				parents[newX][newY] = current;// �O�����`�I
            }
        }
    }

	if (endNode.x != -1 && endNode.y != -1) {// �������I
		vector<pair<int, int>> path;// ²�Ƹ��|
		vector<pair<pair<int, int>, string>> detailedPath;// �ԲӸ��|
		Node current = endNode;// �q���I�}�l
		while (current.parentX != -1 && current.parentY != -1) {// �@�����^����`�I
			path.push_back({ current.x, current.y });// ²�Ƹ��|
			detailedPath.push_back({ {current.x + 1, current.y + 1}, current.direction });// �ԲӸ��|
			current = parents[current.x][current.y];// ����`�I
        }
		path.push_back({ startX, startY });// �_�I
		detailedPath.push_back({ {startX + 1, startY + 1}, "Start" });// �_�I
		reverse(path.begin(), path.end());// ²�Ƹ��|����
		reverse(detailedPath.begin(), detailedPath.end());// �ԲӸ��|����

		printPath(detailedPath, path);// ���L���|
    }
    else {
		cout << "No path to exit found." << endl;// �S�������|
    }

    return 0;
}
