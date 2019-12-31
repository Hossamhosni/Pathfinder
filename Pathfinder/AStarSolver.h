#ifndef ASTAR_SOLVER_H
#define ASTAR_SOLVER_H
#include <vector>
#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#define SIZE 15
using std::vector;
using std::pair;
using std::map;
using std::make_pair;
using std::cout;
using std::endl;

struct Cell {
	int h, g, i, j, f;
	vector<Cell*> neighbors;
	bool obstacle;
	sf::RectangleShape sfrect;

	bool operator < (const Cell& cell) const
	{
		return (f < cell.f);
	}
};
class AStarSolver {
	
private:
	vector<Cell*> openSet;
	vector<Cell*> path;
	vector < vector<Cell*>> grid;
	map<Cell*, Cell*> came_from;
	Cell* end;
	Cell* start;
	void print_path();
public:
	AStarSolver(vector<vector<Cell*>>);
	void solve();
	void setup();
	void print_grid(char s);
	int calc_huristic(pair<int, int>, pair<int, int>);
	void reconstruct_path(Cell*);
	void get_cells_neighbors();
	void draw_grid(sf::RenderWindow*);
	void make_cell_wall(int, int);
	void clear_grid();
	void draw_open_set();
};
#endif
