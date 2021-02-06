#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>

#define ABREVIATIONS
#define STRUCTURES

#ifdef  ABREVIATIONS
#define WALL 0
#define FREE 1
#endif

#ifdef STRUCTURES
class coord2D
{
public:
	int x, y;
	coord2D();
	coord2D(int x_init, int y_init);

	double id()const;
	std::string to_action();
	std::string to_action_raw();
	coord2D operator+(coord2D element);
	bool operator==(const coord2D& position)const;

	friend coord2D operator+(coord2D point, std::pair<std::string, coord2D> action);
	friend coord2D operator+(std::pair<std::string, coord2D> action, coord2D point);
};
inline std::unordered_map<std::string, coord2D> actions_map = {
	{"null",coord2D(-1,-1)},
	{"up",coord2D(-1,0)},
	{"right",coord2D(0,1)},
	{"down",coord2D(1,0)},
	{"left",coord2D(0,-1)}
};
struct node {
	coord2D position, action;
	node* parent;
	int fitness;
	unsigned steps;
};
bool operator<(const node& a, const node& b);
struct DereferenceCompareNode
{
	bool operator()(const node* a, const node* b)const;
};
//hasher:
namespace std
{
	template<>
	struct hash<coord2D>
	{
		size_t operator()(const coord2D& obj) const
		{
			return hash<double>()(obj.id());
		}
	};
}
#endif

class Labyrinth {
public:
	Labyrinth(std::string filename);
	void solve_maze_Astar(std::string filename, bool formatted = false);
	std::vector<std::vector<int>> GetMatrix();
	size_t GetSizeL();
	size_t GetSizeC();
	void SetFrom(coord2D from);
	void SetTo(coord2D to);
	coord2D GetFrom();
	coord2D GetTo();

private:
	int fitness_calculator(coord2D point1, coord2D point2);
	void build_initial_node();
	bool finished(node* evaluated_node);
	bool verify_position(coord2D position, std::unordered_set<coord2D>& closed);
	node* build_node(coord2D position, node* parent, std::string action);
	void expand_node(std::priority_queue<node*, std::vector<node*>, DereferenceCompareNode>& fringe,
		std::unordered_set<coord2D>& closed);
	void print_solution(node* current_node, std::string filename);
	void print_solution_raw(node* current_node, std::string filename);

private:
	std::vector<std::vector<int>> matrix;
	coord2D mouse_position, cheese_position;
	node* mouse;
	size_t sizeL, sizeC;
};
