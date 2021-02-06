#include "Labyrinth.h"

#pragma region STRUCTURES_DEF
coord2D::coord2D()
{
	x = y = 0;
}
coord2D::coord2D(int x_init, int y_init)
{
	x = x_init;
	y = y_init;
}
double coord2D::id() const
{
	return double(x + 10) / double(y + 10);
}
coord2D coord2D::operator+(coord2D element)
{
	return coord2D(this->x + element.x, this->y + element.y);
}
bool coord2D::operator==(const coord2D& position) const
{
	return position.x == x && position.y == y;
}
std::string coord2D::to_action()
{
	if (*this == actions_map["up"])
		return "up";
	else if (*this == actions_map["right"])
		return "right";
	else if (*this == actions_map["down"])
		return "down";
	else if (*this == actions_map["left"])
		return "left";
	else
		return "null";
}
std::string coord2D::to_action_raw()
{
	if (*this == actions_map["up"])
		return "u";
	else if (*this == actions_map["right"])
		return "r";
	else if (*this == actions_map["down"])
		return "d";
	else if (*this == actions_map["left"])
		return "l";
	else
		return "n";
}
coord2D operator+(coord2D point, std::pair<std::string, coord2D> action)
{
	return coord2D(point + action.second);
}
coord2D operator+(std::pair<std::string, coord2D> action, coord2D point)
{
	return coord2D(point + action.second);
}
bool operator<(const node& a, const node& b)
{
	return a.fitness > b.fitness;
}
bool DereferenceCompareNode::operator()(const node* a, const node* b) const
{
	return (*a) < (*b);
}
#pragma endregion 

#pragma region LABYRINTH CLASS
// PRIVATE
int Labyrinth::fitness_calculator(coord2D point1, coord2D point2)
{
	return abs(point1.x - point2.x) + abs(point1.y - point2.y);
}
void Labyrinth::build_initial_node()
{
	mouse = new node;
	mouse->position.x = mouse_position.x;
	mouse->position.y = mouse_position.y;
	mouse->parent = NULL;
	mouse->steps = 0;
	mouse->fitness = fitness_calculator(mouse_position, cheese_position);
	mouse->action = actions_map["null"];
}
bool Labyrinth::finished(node* evaluated_node)
{
	if (evaluated_node == NULL)
		return false;
	return evaluated_node->position == cheese_position;
}
bool Labyrinth::verify_position(coord2D position, std::unordered_set<coord2D>& closed)
{
	if (position.x < 0 || position.y < 0 || position.x >= sizeL || position.y >= sizeC)
		return false;
	if (matrix[position.x][position.y] == WALL)
		return false;
	if (!closed.insert(position).second)
		return false;
	return true;
}
node* Labyrinth::build_node(coord2D position, node* parent, std::string action)
{
	node* new_node;
	new_node = new node;

	new_node->position = position;
	new_node->parent = parent;
	new_node->action = actions_map[action];
	new_node->fitness = fitness_calculator(new_node->position, cheese_position);
	new_node->steps = new_node->parent->steps + 1;

	return new_node;
}
void Labyrinth::expand_node(std::priority_queue<node*, std::vector<node*>, DereferenceCompareNode>& fringe, std::unordered_set<coord2D>& closed)
{
	node* current_node = fringe.top();
	fringe.pop();

	coord2D
		up = current_node->position + actions_map["up"],
		right = current_node->position + actions_map["right"],
		down = current_node->position + actions_map["down"],
		left = current_node->position + actions_map["left"];

	if (verify_position(up, closed))
	{
		node* new_node1 = build_node(up, current_node, "up");
		fringe.push(new_node1);
	}
	if (verify_position(right, closed))
	{
		node* new_node2 = build_node(right, current_node, "right");
		fringe.push(new_node2);
	}
	if (verify_position(down, closed))
	{
		node* new_node3 = build_node(down, current_node, "down");
		fringe.push(new_node3);
	}
	if (verify_position(left, closed))
	{
		node* new_node4 = build_node(left, current_node, "left");
		fringe.push(new_node4);
	}
}
void Labyrinth::print_solution(node* current_node, std::string filename)
{
	//path reconstruction
	std::stack<node> path;
	while (current_node != NULL)
	{
		path.push(*current_node);
		current_node = current_node->parent;
	}

	//path print
	std::ofstream fout(filename);

	fout << "SOLUTION: SHORTEST PATH FROM MOUSE TO CHEESE\n";
	fout << "\nSTEP\tPOSITION\tACTION\n";
	while (!path.empty())
	{
		fout << path.top().steps << "\t\t" << path.top().position.x << "\t" << path.top().position.y << "\t\t" << path.top().action.to_action() << "\n";
		path.pop();
	}

	fout.close();
}
void Labyrinth::print_solution_raw(node* current_node, std::string filename)
{
	//path reconstruction
	std::stack<node> path;
	while (current_node != NULL)
	{
		path.push(*current_node);
		current_node = current_node->parent;
	}

	//path print
	std::ofstream fout(filename);

	while (!path.empty())
	{
		fout << path.top().action.to_action_raw();
		path.pop();
	}

	fout.close();
}

// PUBLIC
Labyrinth::Labyrinth(std::string filename)
{
	std::ifstream fin(filename);

	fin >> sizeL;
	fin >> sizeC;
	for (unsigned i = 0; i < sizeL; i++)
	{
		std::vector<int> line;
		for (unsigned j = 0; j < sizeC; j++)
		{
			int element;
			fin >> element;
			if (element == 2)
			{
				mouse_position.x = i;
				mouse_position.y = j;
			}
			line.push_back(element);
		}
		matrix.push_back(line);
	}

	fin.close();
}

void Labyrinth::solve_maze_Astar(std::string filename, bool formatted)
{
	if (matrix.empty())
		return;

	std::priority_queue<node*, std::vector<node*>, DereferenceCompareNode> fringe;
	std::unordered_set<coord2D> closed;

	build_initial_node();
	fringe.push(mouse);
	closed.insert(mouse->position);

	while (!finished(fringe.empty() ? NULL : fringe.top()))
	{
		if (fringe.empty())
		{
			std::ofstream fout(filename);
			if (formatted)
				fout << "SOLUTION: THERE IS NO PATH FROM MOUSE TO CHEESE\n";
			else
				fout << "@";
			fout.close();
			return;
		}

		expand_node(fringe, closed);
	}

	if (formatted)
		print_solution(fringe.top(), filename);
	else
		print_solution_raw(fringe.top(), filename);
}
std::vector<std::vector<int>> Labyrinth::GetMatrix()
{
	return matrix;
}
size_t Labyrinth::GetSizeL()
{
	return sizeL;
}
size_t Labyrinth::GetSizeC()
{
	return sizeC;
}
void Labyrinth::SetFrom(coord2D from)
{
	mouse_position = from;
}
void Labyrinth::SetTo(coord2D to)
{
	cheese_position = to;
}

coord2D Labyrinth::GetFrom()
{
	return mouse_position;
}

coord2D Labyrinth::GetTo()
{
	return cheese_position;
}
#pragma endregion
