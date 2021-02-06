#include "Graph.h"
#include "Random.h"

#include <fstream>
#include <sstream>
#include <stack>
#include <queue>

Graph::Graph(ReadStyle readStyle, const std::string& fileName) :
	m_inputFileName(fileName),
	m_readStyle(readStyle)
{
	switch (readStyle)
	{
	case ReadStyle::FC:
		readGraphFC();
		break;
	case ReadStyle::FCB:
		readGraphFCB();
		break;
	default:
		throw std::invalid_argument("Undefined read style");
	}
}

Graph::ReadStyle Graph::GetReadStyle() const
{
	return m_readStyle;
}

void Graph::maximumFlowAlgorithm()
{
	calculateResidualValues();

	auto path = pathBetween(m_sourceNode, m_targetNode);
	while (path.has_value())
	{
		const uint16_t rMin = residualMin(path.value());
		updateResidualValues(path.value(), rMin);
		path = pathBetween(m_sourceNode, m_targetNode);
	}
	maximizeFlow();

	initializeArches();
	highlightMinimalCut();
}

void Graph::minimumCostFlowAlgorithm()
{
	calculateResidualValues();

	auto cycle = negativeCycle();
	while (cycle.has_value())
	{
		const uint16_t rMin = residualMin(cycle.value());
		updateResidualValues(cycle.value(), rMin);
		cycle = negativeCycle();
	}
	minimizeFlow();

	initializeArches();
}

uint16_t Graph::totalCost() const
{
	uint16_t cost = 0;
	for (const auto& line : m_adjacencyMatrix)
	{
		for (const auto& arch : line)
		{
			if (arch.c)
			{
				cost += arch.f * arch.b;
			}
		}
	}
	return cost;
}

void Graph::prepareGraphForDraw(const sf::RenderWindow& window)
{
	initializeNodes(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
	initializeArches();
}

void Graph::drawGraph(sf::RenderWindow& window)
{
	for (const auto& linker : m_archList)
		linker.drawArchLine(window);
	for (const auto& node : m_nodeList)
		node.drawNode(window);
	for (const auto& linker : m_archList)
		linker.drawArchArrowHead(window);
}

void Graph::readGraphFC()
{
	std::ifstream inputFile(m_inputFileName);
	std::string line;

	// Resize
	uint16_t nodesCount;
	inputFile >> nodesCount >> m_sourceNode >> m_targetNode;
	inputFile.get();
	m_adjacencyMatrix.resize(nodesCount);
	m_adjacencyList.resize(nodesCount);
	std::for_each(m_adjacencyMatrix.begin(), m_adjacencyMatrix.end(), [nodesCount](std::vector<ArchProperties>& line) {line.resize(nodesCount); });

	// Read
	while (std::getline(inputFile, line))
	{
		std::stringstream sStream;
		sStream.str(line);

		ArchProperties arch;
		uint16_t x, y;
		sStream >> x >> y >> arch.f >> arch.c;

		m_adjacencyMatrix[x][y] = arch;
		m_adjacencyList[x].push_back(y);
	}

	inputFile.close();
}

void Graph::readGraphFCB()
{
	std::ifstream inputFile(m_inputFileName);
	std::string line;

	// Resize
	uint16_t nodesCount;
	inputFile >> nodesCount >> m_sourceNode >> m_targetNode >> m_value;
	inputFile.get();
	m_adjacencyMatrix.resize(nodesCount);
	m_adjacencyList.resize(nodesCount);
	std::for_each(m_adjacencyMatrix.begin(), m_adjacencyMatrix.end(), [nodesCount](std::vector<ArchProperties>& line) {line.resize(nodesCount); });

	// Read
	while (std::getline(inputFile, line))
	{
		std::stringstream sStream;
		sStream.str(line);

		ArchProperties arch;
		uint16_t x, y;
		sStream >> x >> y >> arch.f >> arch.c >> arch.b;

		m_adjacencyMatrix[x][y] = arch;
		m_adjacencyList[x].push_back(y);
	}

	inputFile.close();
}

void Graph::initializeNodes(float windowWidth, float windowHeight)
{
	m_nodeList.clear();
	Random randomizer;
	size_t nrOfNodes = m_adjacencyMatrix.size();

	uint16_t nodeID = 0;
	while (nrOfNodes--)
	{
		float randomX, randomY;
		bool positionIsCorrect;
		do
		{
			positionIsCorrect = true;
			randomX = randomizer.generate(g_radius, windowWidth - g_radius);
			randomY = randomizer.generate(g_radius + 160, windowHeight - g_radius);
			for (const auto& node : m_nodeList)
				if (node.isTooClose(randomX, randomY))
					positionIsCorrect = false;
		} while (!positionIsCorrect);


		m_nodeList.emplace_back(nodeID++, randomX, randomY, sf::Color(20, 150, 20), 20.0f);

	}
}

void Graph::initializeArches()
{
	m_archList.clear();
	for (auto& sourceNode : m_nodeList)
	{
		std::vector<Node*> nodesToLink;
		const auto nodeID = sourceNode.GetID();

		for (const auto& element : m_adjacencyList[nodeID])
		{
			nodesToLink.push_back(&m_nodeList[element]);
		}

		for (auto destinationNode : nodesToLink)
		{
			if (sourceNode.GetID() == destinationNode->GetID())
			{
				Arch arch(0, sourceNode.getPosition(), sf::Color::Red);
				m_archList.push_back(arch);
			}
			else
			{
				auto archPropreties = m_adjacencyMatrix[sourceNode.GetID()][destinationNode->GetID()];
				const std::string archText(std::to_string(archPropreties.f) + "," + std::to_string(archPropreties.c) + (archPropreties.b != 0 ? "," + std::to_string(archPropreties.b) : ""));
				Arch arch(0, archText, sourceNode.getPosition(), destinationNode->getPosition(), sf::Color(200, 200, 20));
				m_archList.push_back(arch);
			}
		}
	}
}

void Graph::calculateResidualValues()
{
	for (size_t nodeX = 0; nodeX < m_adjacencyMatrix.size(); ++nodeX)
	{
		for (size_t nodeY = 0; nodeY < m_adjacencyMatrix.size(); ++nodeY)
		{
			m_adjacencyMatrix[nodeX][nodeY].r = m_adjacencyMatrix[nodeX][nodeY].c - m_adjacencyMatrix[nodeX][nodeY].f + m_adjacencyMatrix[nodeY][nodeX].f;
			if (m_adjacencyMatrix[nodeX][nodeY].c == 0 && m_adjacencyMatrix[nodeX][nodeY].r != 0)
				m_adjacencyMatrix[nodeX][nodeY].b = -m_adjacencyMatrix[nodeY][nodeX].b;
		}
	}
}

std::optional<std::vector<uint16_t>> Graph::pathBetween(uint16_t source, uint16_t target)
{
	std::optional<std::vector<uint16_t>> path;
	std::stack<uint16_t> stack;
	std::vector<bool> visited(m_adjacencyMatrix.size(), false);
	stack.push(source);
	visited[source] = true;

	while (!stack.empty() && visited[target] == false)
	{
		bool stop = false;
		for (uint16_t node = 0; node < m_adjacencyMatrix.size() && stop == false; ++node)
		{
			if (m_adjacencyMatrix[stack.top()][node].r && visited[node] == false)
			{
				stop = true;
				visited[node] = true;
				stack.push(node);
			}
		}
		if (stop == false)
		{
			stack.pop();
		}
	}

	if (!stack.empty())
	{
		path.emplace();
		while (!stack.empty())
		{
			path->push_back(stack.top());
			stack.pop();
		}
		std::reverse(path->begin(), path->end());
	}

	return path;
}

uint16_t Graph::residualMin(const std::vector<uint16_t>& path)
{
	uint16_t min = m_adjacencyMatrix[path[0]][path[1]].r;
	for (size_t node = 1; node < path.size() - 1; ++node)
	{
		if (m_adjacencyMatrix[path[node]][path[node + 1]].r < min)
		{
			min = m_adjacencyMatrix[path[node]][path[node + 1]].r;
		}
	}
	return min;
}

void Graph::updateResidualValues(const std::vector<uint16_t>& path, uint16_t rMin)
{
	for (size_t node = 0; node < path.size() - 1; ++node)
	{
		m_adjacencyMatrix[path[node]][path[node + 1]].r -= rMin;
		m_adjacencyMatrix[path[node + 1]][path[node]].r += rMin;
	}
}

void Graph::maximizeFlow()
{
	for (size_t nodeX = 0; nodeX < m_adjacencyMatrix.size(); ++nodeX)
	{
		for (size_t nodeY = 0; nodeY < m_adjacencyMatrix.size(); ++nodeY)
		{
			m_adjacencyMatrix[nodeX][nodeY].f = m_adjacencyMatrix[nodeX][nodeY].c - m_adjacencyMatrix[nodeX][nodeY].r;
		}
	}
}

void Graph::highlightMinimalCut()
{
	std::queue<uint16_t> queue;
	std::vector<bool> visited(m_adjacencyMatrix.size(), false);
	queue.push(m_sourceNode);
	visited[m_sourceNode] = true;

	while (!queue.empty())
	{
		for (size_t node = 0; node < m_adjacencyMatrix.size(); ++node)
		{
			if (m_adjacencyMatrix[queue.front()][node].r && visited[node] == false)
			{
				queue.push(node);
				visited[node] = true;
			}
		}
		queue.pop();
	}

	for (size_t node = 0; node < visited.size(); ++node)
	{
		if (visited[node])
			m_nodeList[node].setFillColor(sf::Color(20, 200, 200));
	}
}

std::optional<std::vector<uint16_t>> Graph::negativeCycle()
{
	std::optional<std::vector<uint16_t>> cycle;

	// Bellman Ford Algorithm
	std::vector<std::optional<uint16_t>> predecessors(m_adjacencyMatrix.size(), std::nullopt);
	std::vector<int> distances(m_adjacencyMatrix.size(), INT_MAX);
	distances[m_sourceNode] = 0;

	bool changed = true;
	uint16_t nodeChanged;
	for (size_t iteration = 0; iteration < m_adjacencyMatrix.size(); ++iteration)
	{
		changed = false;
		for (size_t nodeX = 0; nodeX < m_adjacencyMatrix.size(); ++nodeX)
		{
			for (size_t nodeY = 0; nodeY < m_adjacencyMatrix.size(); ++nodeY)
			{
				if (m_adjacencyMatrix[nodeX][nodeY].r)
				{
					if (distances[nodeY] > (distances[nodeX] == INT_MAX ? INT_MAX : distances[nodeX] + m_adjacencyMatrix[nodeX][nodeY].b))
					{
						distances[nodeY] = distances[nodeX] + m_adjacencyMatrix[nodeX][nodeY].b;
						predecessors[nodeY].emplace();
						predecessors[nodeY] = nodeX;
						changed = true;
						nodeChanged = nodeY;
					}
				}
			}
		}
	}

	// Build negative cycle
	if (changed)
	{
		cycle.emplace();
		cycle.value() = buildNegativeCycle(nodeChanged);
	}

	return cycle;
}

void Graph::minimizeFlow()
{
	for (size_t nodeX = 0; nodeX < m_adjacencyMatrix.size(); ++nodeX)
	{
		for (size_t nodeY = 0; nodeY < m_adjacencyMatrix.size(); ++nodeY)
		{
			m_adjacencyMatrix[nodeX][nodeY].f = abs(m_adjacencyMatrix[nodeX][nodeY].c - m_adjacencyMatrix[nodeX][nodeY].r);
		}
	}
}

std::vector<uint16_t> Graph::buildNegativeCycle(uint16_t startNode)
{
	std::vector<uint16_t> cycle;
	std::stack<uint16_t> stack;
	std::vector<bool> visited(m_adjacencyMatrix.size(), false);
	std::vector<bool> blocked(m_adjacencyMatrix.size(), false);
	stack.push(startNode);

	int cycleValue = 0;
	while (!stack.empty() && visited[startNode] == false)
	{
		bool stop = false;
		for (uint16_t node = 0; node < m_adjacencyMatrix.size() && stop == false; ++node)
		{
			if (m_adjacencyMatrix[stack.top()][node].r && visited[node] == false && blocked[node] == false)
			{
				if (node == startNode && cycleValue + m_adjacencyMatrix[stack.top()][node].b >= 0)
					continue;

				cycleValue += m_adjacencyMatrix[stack.top()][node].b;

				stop = true;
				visited[node] = true;
				stack.push(node);
			}
		}
		if (stop == false)
		{
			if (stack.size() == 2)
			{
				blocked[stack.top()] = true;
				visited.assign(m_adjacencyMatrix.size(), false);
			}

			const auto lastNode = stack.top();
			stack.pop();
			cycleValue -= m_adjacencyMatrix[stack.top()][lastNode].b;
		}
	}

	if (!stack.empty())
	{
		while (!stack.empty())
		{
			cycle.push_back(stack.top());
			stack.pop();
		}
		std::reverse(cycle.begin(), cycle.end());
	}

	return cycle;
}
