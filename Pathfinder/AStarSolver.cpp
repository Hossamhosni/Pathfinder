#include "AStarSolver.h"
AStarSolver::AStarSolver(vector<vector<Cell*>> grid) {
	this->grid = grid;

}
void AStarSolver::setup() {
	for (auto i = grid.begin(); i != grid.end(); i++) {
		*i = vector<Cell*>(SIZE);
	}

	for (auto i = grid.begin(); i != grid.end(); i++) {
		vector<Cell*> row = *i;
		for (auto j = i->begin(); j != i->end(); j++) {
			int row = i - grid.begin();
			int col = j - i->begin();
			*j = new Cell;
			Cell* cell = *j;
			cell->f = 100000;
			cell->g = 100000;
			cell->h = calc_huristic(make_pair(row, col), make_pair(SIZE - 1, SIZE - 1));
			cell->i = row;
			cell->j = col;
			cell->obstacle = false;
			cell->sfrect.setSize(sf::Vector2f(40, 40));
			cell->sfrect.setOutlineThickness(1);
			cell->sfrect.setOutlineColor(sf::Color::Magenta);
			cell->sfrect.setPosition(row * 40, col * 40);
		}
	}

	this->start = this->grid[0][0];
	this->end = this->grid[SIZE - 1][SIZE - 1];
	start->f = start->h;
	start->g = 0;

}

void AStarSolver::print_grid(char s) {
	for (unsigned int i = 0; i < (SIZE * 5 + 1); i++) {
		cout << "-";
	}
	cout << endl;
	for (auto i = grid.begin(); i != grid.end(); i++) {
		cout << "|" << " ";
		vector<Cell*> row = *i;
		for (auto j = i->begin(); j != i->end(); j++) {
			if (s == 'g')
				cout << (*j)->g << " | ";
			else if (s == 'h') 
				cout << (*j)->h << " | ";
			else 
				cout << (*j)->f << " | ";
		}
		cout << endl;
	}
	for (unsigned int i = 0; i < (SIZE * 5 + 1); i++) {
		cout << "-";
	}
	cout << endl;
}


int AStarSolver::calc_huristic(pair<int, int> start, pair<int, int> end) {
	return sqrt(pow(start.first - end.first, 2) + pow(start.second - end.second, 2)); // calculate the eucledian distance as huristic
}

void AStarSolver::solve() {
	openSet.clear();
	get_cells_neighbors();
	start = this->grid[0][0];
	openSet.push_back(start); // Push the start node to the open set
	while (this->openSet.size() > 0) {
		sort(openSet.begin(), openSet.end());
		draw_open_set();
		Cell *current = *(openSet.begin());
		if (current == end) {
			reconstruct_path(current);
			print_grid('g');
			print_grid('f');
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
					if ((*i)->i == n->i && (*i)->j == n->j) found = true;
				}
				if (!found) {
					openSet.push_back(*neighbor);
				}
			}
		}
		openSet.erase(openSet.begin());

	}
	
	cout << "NO SOLUTION" << endl;
}
void AStarSolver::get_cells_neighbors() {
	for (auto i = grid.begin(); i != grid.end(); i++) {
		for (auto j = i->begin(); j != i->end(); j++) {
			int row = i - grid.begin();
			int col = j - i->begin();
			Cell *cell = *j;
			cell->neighbors.resize(0);
			if (row - 1 >= 0 && row - 1 < SIZE && !(grid[row - 1][col]->obstacle)) {
				cell->neighbors.push_back(grid[row - 1][col]);
			}
			if (row + 1 >= 0 && row + 1 < SIZE && !(grid[row + 1][col]->obstacle)) {
				cell->neighbors.push_back(grid[row + 1][col]);
			}
			if (col - 1 >= 0 && col - 1 < SIZE && !(grid[row][col - 1]->obstacle)) {
				cell->neighbors.push_back(grid[row][col - 1]);
			}
			if (col + 1 >= 0 && col + 1 < SIZE && !(grid[row][col + 1]->obstacle)) {
				cell->neighbors.push_back(grid[row][col + 1]);
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
		cout << "(" << (*i)->i << ", " << (*i)->j <<" H = " << (*i)->h << " - F = " << (*i)->f << ")" << endl;
		(*i)->sfrect.setFillColor(sf::Color::Green);
	}
}


void AStarSolver::draw_grid(sf::RenderWindow* win) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			win->draw(grid[j][i]->sfrect);
		}
	}
}
void AStarSolver::make_cell_wall(int i, int j) {
	grid[j][i]->obstacle = true;
	grid[j][i]->sfrect.setFillColor(sf::Color::Black);
}

void AStarSolver::clear_grid() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++)
		{
			delete grid[i][j];
		}
	}
	setup();
}

void AStarSolver::draw_open_set() {
	for (auto i = openSet.begin(); i != openSet.end(); i++) {
		cout << "I = " << (*i)->i << " J = " << (*i)->j << "   ";
	}
	cout << endl;
}