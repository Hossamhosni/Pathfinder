#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <string>
#include <iomanip>
#define SIZE 5
using namespace std;

struct Cell {
	int h, g, i, j, f;
	vector<Cell*> neighbors;
	bool obstacle;

	bool operator < (const Cell& cell) const
	{
		return (f < cell.f);
	}
};



class AStarSolver {
private:
	vector<Cell*> openSet;
	vector<Cell*> path;
	vector < vector<Cell>> grid;
	map<Cell*, Cell*> came_from;
	Cell* end;
	Cell* start;
	void print_path();
public:
	AStarSolver(vector<vector<Cell>>);
	void solve();
	void setup();
	void print_grid();
	void assign_cell(int, int, Cell);
	int calc_huristic(pair<int, int>, pair<int, int>);
	void reconstruct_path(Cell*);
	void get_cells_neighbors();
	void put_obstacles();
};



AStarSolver::AStarSolver(vector<vector<Cell>> grid) {
	this->grid = grid;
	
}
void AStarSolver::setup() {
	for (auto i = grid.begin(); i != grid.end(); i++) {
		*i = vector<Cell>(SIZE);
	}

	for (auto i = grid.begin(); i != grid.end(); i++) {
		vector<Cell> row = *i;
		for (auto j = i->begin(); j != i->end(); j++) {
			int row = i - grid.begin();
			int col = j - i->begin();
			j->f = INT_MAX - 1;
			j->g = INT_MAX - 1;
			j->h = calc_huristic(make_pair(row, col), make_pair(SIZE - 1, SIZE - 1));
			j->i = row;
			j->j = col;
		}
	}
	put_obstacles();
	get_cells_neighbors();
	this->start = &this->grid[0][0];
	this->end = &this->grid[SIZE - 1][SIZE - 1];
	start->f = start->h;
	start->g = 0;
	
}

void AStarSolver::print_grid() {
	for (unsigned int i = 0; i < (SIZE * 5 + 1); i++) {
		cout << "-";
	}
	cout << endl;
	for (auto i = grid.begin(); i != grid.end(); i++) {
		cout << "|" << " ";
		vector<Cell> row = *i;
		for (auto j = i->begin(); j != i->end(); j++) {
			cout  << j->f << " | ";
		}
		cout << endl;
	}
	for (unsigned int i = 0; i < (SIZE * 5 + 1); i++) {
		cout << "-";
	}
	cout << endl;
}

void AStarSolver::assign_cell(int i, int j, Cell c) {
	this->grid[i][j] = c;
}

int AStarSolver::calc_huristic(pair<int, int> start, pair<int, int> end) {
	return sqrt(pow(start.first - end.first, 2) + pow(start.second - end.second, 2)); // calculate the eucledian distance as huristic
}

void AStarSolver::solve() {
	
	setup();
	start = &this->grid[0][0];
	openSet.push_back(start); // Push the start node to the open set
	while (this->openSet.size() > 0) {
		sort(openSet.begin(), openSet.end());
		Cell *current = *(openSet.begin());
		if (current->i == end->i && current->j == end->j) {
			reconstruct_path(current);
			return;
		}
		
		for (auto neighbor = current->neighbors.begin(); neighbor != current->neighbors.end(); neighbor++) {
			int score = current->g + 1; // 1 is the distance between each cell and neigbors by default
			Cell* n = *neighbor;
			if (score < n->g) {
				came_from[n] = current;
				n->g = score;
				n->f = n->g + n->h;
				bool found = false;
				for (auto i = openSet.begin(); i != openSet.end(); i++) {
					if ((*i)->i == current->i && (*i)->j == current->j) found = false;
				}
				if (!found) {
					openSet.push_back(*neighbor);
				}
			}
		}
		openSet.erase(openSet.begin());
		print_grid();
	} 
	cout << "NO SOLUTION" << endl;
}
void AStarSolver::get_cells_neighbors() {
	for (auto i = grid.begin(); i != grid.end(); i++) {
		for (auto j = i->begin(); j != i->end(); j++) {
			int row = i - grid.begin();
			int col = j - i->begin();
			if (row - 1 >= 0 && row - 1 < SIZE && !(grid[row - 1][col].obstacle)) {
				j->neighbors.push_back(&grid[row - 1][col]);
			}
			if (row + 1 >= 0 && row + 1 < SIZE && !(grid[row + 1][col].obstacle)) {
				j->neighbors.push_back(&grid[row + 1][col]);
			}
			if (col - 1 >= 0 && col - 1 < SIZE && !(grid[row][col - 1].obstacle)) {
				j->neighbors.push_back(&grid[row][col - 1]);
			}
			if (col + 1 >= 0 && col + 1 < SIZE && !(grid[row][col + 1].obstacle)) {
				j->neighbors.push_back(&grid[row][col + 1]);
			}
		}
	}
}
void AStarSolver::reconstruct_path(Cell* current) {
	path = { current };
	while (current != start) {
		current = came_from[current];
		path.push_back(current);
	}
	reverse(path.begin(), path.end());
	print_path();
}

void AStarSolver::print_path() {
	cout << "The Path is \n";
	for (auto i = path.begin(); i != path.end(); i++) {
		cout << "(" << (*i)->i << ", " << (*i)->j << ")" << endl;
	}
}

void AStarSolver::put_obstacles() {
	this->grid[0][1].obstacle = true;
	this->grid[0][2].obstacle = true;
	this->grid[0][3].obstacle = true;
	this->grid[1][3].obstacle = true;
	this->grid[2][1].obstacle = true;
	this->grid[3][1].obstacle = true;
	this->grid[3][2].obstacle = true;
	this->grid[3][3].obstacle = true;
}

int main() {
	vector<vector<Cell>> grid(SIZE);
	AStarSolver solver(grid);
	solver.solve();
	int n;
	cin >> n;
	return 0;
}