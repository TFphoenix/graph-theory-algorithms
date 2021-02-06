#include "Graph.h"
#include "Random.h"
#include "DisjointSet.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

bool Graph::DFS_Result::empty()
{
	return t1.empty();
}

Graph::Graph(ReadStyle readStyle, const std::string& fileName)
{
	m_inputFileName = fileName;
	m_isWUndirected = false;

	switch (readStyle)
	{
	case ReadStyle::AdjacencyList:
		readGraphAsList();
		break;
	case ReadStyle::AdjacencyMatrix:
		readGraphAsMatrix();
		break;
	case ReadStyle::Undirected:
		m_isWUndirected = true;
		readGraphUndirected();
		break;
	default:
		break;
	}

	if (!m_isWUndirected)
	{
		m_hasCircuits = hasCircuits();
		m_DFS = dfsClassic();
		topologicalSort();
		generateTransposedGraph();
		generateUndirectedGraph();
		dfsCC();
		dfsSCC();
		generateSCCList();
	}
	else
	{
		fromWUMatrixToLinkerList();
	}
}

void Graph::prepareGraphForDraw(const sf::RenderWindow& window)
{
	initializeNodes(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y), m_adjacencyList, m_nodeList);
	initializeLinkers(m_adjacencyList, m_nodeList, m_nodeLinkerList);
}

void Graph::prepareTopoForDraw(const sf::RenderWindow& window)
{
	if (m_hasCircuits)
		return;
	initializeNodes(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y), m_adjacencyList, m_nodeList, NodeInit::Topo);
	initializeLinkers(m_adjacencyList, m_nodeList, m_nodeLinkerList);
}

void Graph::prepareSCCForDraw(const sf::RenderWindow& window)
{
	initializeNodes(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y), m_sccAdjacencyList, m_sccNodeList, NodeInit::SCC);
	initializeLinkers(m_sccAdjacencyList, m_sccNodeList, m_sccNodeLinkerList);
}

void Graph::prepareUndirectedGraphForDraw(const sf::RenderWindow& window)
{
	initializeNodes(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y), m_undirectedAdjacencyList, m_undirectedNodeList, NodeInit::Undirected);
	initializeLinkers(m_undirectedAdjacencyList, m_undirectedNodeList, m_undirectedNodeLinkerList);
}

void Graph::prepareMSTForDraw(const sf::RenderWindow& window)
{
	initializeLinkers(m_mstAdjacencyList, m_undirectedNodeList, m_mstNodeLinkerList);
}

void Graph::drawGraph(sf::RenderWindow& window)
{
	for (const auto& linker : m_nodeLinkerList)
		linker.drawLinkerLine(window);
	for (const auto& node : m_nodeList)
		node.drawNode(window);
	for (const auto& linker : m_nodeLinkerList)
		linker.drawLinkerArrowHead(window);
}

void Graph::drawTopo(sf::RenderWindow& window)
{
	if (m_hasCircuits)
	{
		// Font
		sf::Font font;
		font.loadFromFile("font.ttf");

		// Text
		sf::Text errorText("ERROR: Can't draw topological sorted graph because it contains circuits", font, 20);
		errorText.setFillColor(sf::Color::White);
		errorText.setStyle(sf::Text::Style::Bold);

		window.draw(errorText);
		return;
	}

	for (const auto& linker : m_nodeLinkerList)
		linker.drawLinkerLine(window);
	for (const auto& node : m_nodeList)
		node.drawNode(window);
	for (const auto& linker : m_nodeLinkerList)
		linker.drawLinkerArrowHead(window);
}

void Graph::drawSCC(sf::RenderWindow& window)
{
	for (const auto& linker : m_sccNodeLinkerList)
		linker.drawLinkerLine(window);
	for (const auto& node : m_sccNodeList)
		node.drawNode(window);
	for (const auto& linker : m_sccNodeLinkerList)
		linker.drawLinkerArrowHead(window);
}

void Graph::drawUndirectedGraph(sf::RenderWindow& window)
{
	for (const auto& linker : m_undirectedNodeLinkerList)
		linker.drawLinkerLine(window);
	for (const auto& node : m_undirectedNodeList)
		node.drawNode(window);
}

void Graph::drawMST(sf::RenderWindow& window)
{
	for (const auto& linker : m_mstNodeLinkerList)
		linker.drawLinkerLine(window);
	for (const auto& node : m_undirectedNodeList)
		node.drawNode(window);
}

bool Graph::GetHasCircuits() const
{
	return m_hasCircuits;
}

std::vector<uint16_t> Graph::GetTopologicalOrder() const
{
	if (m_hasCircuits)
		throw "ERROR: Can't apply topological sort on a graph that has circuits";
	return m_topologicalOrder;
}

Graph::ListT Graph::GetConnectedComponents() const
{
	return m_connectedComponents;
}

Graph::ListT Graph::GetStronglyConnectedComponents() const
{
	return m_stronglyConnectedComponents;
}

void Graph::PrimAlgorithm()
{
	const size_t nodesCount = m_wUndirectedAdjacencyMatrix.size();
	std::vector<int16_t> visited(nodesCount, INT16_MAX);
	std::vector<uint16_t> nodes = { 0 };
	visited[0] = 0;

	while (nodes.size() != nodesCount)
	{
		std::pair<uint16_t, uint16_t> minimumLink;
		int16_t minimumLinkValue = INT16_MAX;

		for (uint16_t node : nodes)
		{
			for (uint16_t nodeIndex = 0; nodeIndex < nodesCount; nodeIndex++)
			{
				const auto thisLinkValue = m_wUndirectedAdjacencyMatrix[node][nodeIndex];
				if (visited[nodeIndex] == INT16_MAX && thisLinkValue != -1)
				{
					if (thisLinkValue < minimumLinkValue)
					{
						minimumLinkValue = thisLinkValue;
						minimumLink = std::make_pair(node, nodeIndex);
					}
				}
			}
		}

		visited[minimumLink.second] = minimumLinkValue;
		nodes.push_back(minimumLink.second);
		m_minimumSpanningTree.push_back(minimumLink);
	}

	generateMSTList();
}

void Graph::KruskalAlgorithm()
{
	LinkListT sortedLinkers(m_wULinkerList);
	LinkListT mstLinkers;
	DisjointSet linkersSet;
	const auto nrOfNodes = m_wUndirectedAdjacencyMatrix.size();
	linkersSet.MakeSet(nrOfNodes);

	std::sort(sortedLinkers.begin(), sortedLinkers.end());

	for (const auto& linker : sortedLinkers)
	{
		if (linkersSet.Find(linker.x) != linkersSet.Find(linker.y))
		{
			linkersSet.Union(linker.x, linker.y);
			mstLinkers.push_back(linker);
		}

		if (mstLinkers.size() == nrOfNodes - 1)
		{
			fromLinkerListToMST(mstLinkers);
			return;
		}
	}
}

void Graph::readGraphAsList()
{
	std::ifstream inputFile(m_inputFileName);
	std::string line;

	while (std::getline(inputFile, line))
	{
		std::stringstream sStream;
		sStream.str(line);

		std::vector<uint16_t> links;
		uint16_t link;
		while (sStream >> link)
			if (link != '@')
				links.push_back(link);

		m_adjacencyList.push_back(links);
	}
	inputFile.close();

	fromListToMatrix();
}

void Graph::readGraphAsMatrix()
{
	std::ifstream inputFile(m_inputFileName);
	std::string line;

	while (std::getline(inputFile, line))
	{
		std::stringstream sStream;
		sStream.str(line);

		std::vector<bool> elements;
		bool element;
		while (sStream >> element)
			elements.push_back(element);
		m_adjacencyMatrix.push_back(elements);
	}
	inputFile.close();

	fromMatrixToList();
}

void Graph::readGraphUndirected()
{
	std::ifstream inputFile(m_inputFileName);
	std::string line;

	while (std::getline(inputFile, line))
	{
		std::stringstream sStream;
		sStream.str(line);

		std::vector<int16_t> elements;
		int16_t element;
		while (sStream >> element)
			elements.push_back(element);
		m_wUndirectedAdjacencyMatrix.push_back(elements);
	}
	inputFile.close();

	fromWUMatrixToList();
}

void Graph::fromListToMatrix()
{
	const auto listSize = m_adjacencyList.size();
	m_adjacencyMatrix.resize(listSize);
	for (size_t iterator = 0; iterator < listSize; ++iterator)
		m_adjacencyMatrix[iterator].resize(listSize, 0);

	size_t lineIndex = 0;
	for (const auto& list : m_adjacencyList)
	{
		for (const auto& link : list)
			m_adjacencyMatrix[lineIndex][link] = 1;
		++lineIndex;
	}
}

void Graph::fromMatrixToList()
{
	const auto matrixSize = m_adjacencyMatrix.size();
	m_adjacencyList.resize(matrixSize);

	for (size_t lineIterator = 0; lineIterator < matrixSize; ++lineIterator)
		for (size_t columnIterator = 0; columnIterator < matrixSize; ++columnIterator)
			if (m_adjacencyMatrix[lineIterator][columnIterator])
				m_adjacencyList[lineIterator].push_back(columnIterator);
}

void Graph::fromWUMatrixToList()
{
	const size_t matrixSize = m_wUndirectedAdjacencyMatrix.size();
	m_undirectedAdjacencyList.resize(matrixSize);

	for (size_t lineIndex = 0; lineIndex < matrixSize; lineIndex++)
	{
		for (size_t columnIndex = 0; columnIndex < matrixSize; columnIndex++)
		{
			if (m_wUndirectedAdjacencyMatrix[lineIndex][columnIndex] != -1)
				m_undirectedAdjacencyList[lineIndex].push_back(columnIndex);
		}
	}
}

void Graph::fromWUMatrixToLinkerList()
{
	const auto size = m_wUndirectedAdjacencyMatrix.size();
	for (size_t line = 0; line < size; ++line)
	{
		for (size_t column = 0; column < line; ++column)
		{
			if (m_wUndirectedAdjacencyMatrix[line][column] != -1)
				m_wULinkerList.emplace_back(Link(m_wUndirectedAdjacencyMatrix[line][column], line, column));
		}
	}
}

void Graph::fromLinkerListToMST(const LinkListT& linkerList)
{
	m_mstAdjacencyList.resize(m_wUndirectedAdjacencyMatrix.size());

	for (const auto& linker : linkerList)
	{
		m_mstAdjacencyList[linker.x].push_back(linker.y);
	}
}

void Graph::initializeNodes(float windowWidth, float windowHeight, const ListT& adjacencyList, std::vector<Node>& nodeList, Graph::NodeInit nodeInit)
{
	Random randomizer;
	size_t nrOfNodes = adjacencyList.size();

	uint16_t nodeID = 0;
	while (nrOfNodes--)
	{
		float randomX, randomY;
		bool positionIsCorrect;
		do
		{
			positionIsCorrect = true;
			randomX = randomizer.generate(g_radius, windowWidth - g_radius);
			randomY = randomizer.generate(g_radius, windowHeight - g_radius);
			for (const auto& node : nodeList)
				if (node.isTooClose(randomX, randomY))
					positionIsCorrect = false;
		} while (!positionIsCorrect);

		switch (nodeInit)
		{
		case NodeInit::SCC:
			nodeList.emplace_back(Node(nodeID++, fromListToText(m_stronglyConnectedComponents[nodeID]), randomX, randomY, sf::Color(20, 150, 20), 50));
			break;
		case NodeInit::Topo:
			nodeList.emplace_back(Node(nodeID++, fromTopoToText(nodeID), randomX, randomY, sf::Color(20, 150, 20), 20));
			break;
		default:
			nodeList.emplace_back(Node(nodeID++, randomX, randomY, sf::Color(20, 150, 20), 20));
			break;
		}
	}
}

void Graph::initializeLinkers(const ListT& adjacencyList, std::vector<Node>& nodeList, std::vector<NodeLinker>& nodeLinkerList)
{
	for (auto& sourceNode : nodeList)
	{
		std::vector<Node*> nodesToLink;
		const auto nodeID = sourceNode.GetID();

		for (const auto& element : adjacencyList[nodeID])
		{
			nodesToLink.push_back(&nodeList[element]);
		}

		for (auto destinationNode : nodesToLink)
		{
			if (sourceNode.GetID() == destinationNode->GetID())
			{
				NodeLinker linker(0, sourceNode.GetPosition(), sf::Color::Red);
				nodeLinkerList.push_back(linker);
			}
			else
			{
				NodeLinker linker(0, sourceNode.GetPosition(), destinationNode->GetPosition(), sf::Color::Red);
				nodeLinkerList.push_back(linker);
			}
		}
	}
}

bool Graph::expandNode(std::stack<uint16_t>& container, const ListT& adjacencyList, std::vector<uint16_t>& visitedTimes, uint16_t& startTime, bool deleteTop)
{
	const uint16_t current_node = container.top();
	if (deleteTop)
		container.pop();

	for (const auto& linkNode : adjacencyList[current_node])
	{
		if (visitedTimes[linkNode] == 0)
		{
			visitedTimes[linkNode] = startTime++;
			container.push(linkNode);
			return true;
		}
	}

	return false;
}

bool Graph::expandNode(std::queue<uint16_t>& container, const ListT& adjacencyList, std::vector<uint16_t>& visitedTimes, uint16_t& startTime, bool deleteTop)
{
	bool successfullyExpanded = false;
	const uint16_t current_node = container.front();
	if (deleteTop)
		container.pop();

	for (const auto& linkNode : adjacencyList[current_node])
	{
		if (visitedTimes[linkNode] == 0)
		{
			successfullyExpanded = true;
			visitedTimes[linkNode] = startTime++;
			container.push(linkNode);
		}
	}

	return successfullyExpanded;
}

bool Graph::pathBetween(uint16_t from, uint16_t to) const
{
	std::queue <uint16_t> nodesQueue;
	std::vector<uint16_t> visitedTimes(m_adjacencyList.size());
	uint16_t time = 1;

	if (from != to)
		visitedTimes[from] = time++;

	nodesQueue.push(from);
	while (!nodesQueue.empty())
		expandNode(nodesQueue, m_adjacencyList, visitedTimes, time, true);

	if (visitedTimes[to])
		return true;
	return false;
}

Graph::DFS_Result Graph::dfsClassic()
{
	if (m_adjacencyList.empty())
		return DFS_Result();

	std::stack<uint16_t> nodesStack;
	uint16_t currentTime = 1;
	DFS_Result times;
	times.t1.resize(m_adjacencyList.size());
	times.t2.resize(m_adjacencyList.size());

	auto nextNodeIt = std::find(times.t2.begin(), times.t2.end(), 0);
	while (nextNodeIt != std::end(times.t2))
	{
		uint16_t nextNode = std::distance(times.t2.begin(), nextNodeIt);
		nodesStack.push(nextNode);
		times.t1[nextNode] = currentTime++;

		while (!nodesStack.empty())
		{
			while (expandNode(nodesStack, m_adjacencyList, times.t1, currentTime, false));
			times.t2[nodesStack.top()] = currentTime++;
			nodesStack.pop();
		}

		nextNodeIt = std::find(times.t2.begin(), times.t2.end(), 0);
	}

	return times;
}

void Graph::dfsCC()
{
	if (m_undirectedAdjacencyList.empty())
		return;

	std::stack<uint16_t> nodesStack;
	uint16_t currentTime = 1;
	DFS_Result times;
	times.t1.resize(m_undirectedAdjacencyList.size());
	times.t2.resize(m_undirectedAdjacencyList.size());

	auto nextNodeIt = std::find(times.t2.begin(), times.t2.end(), 0);
	while (nextNodeIt != std::end(times.t2))
	{
		uint16_t nextNode = std::distance(times.t2.begin(), nextNodeIt);
		const uint16_t timeSnap = currentTime;
		nodesStack.push(nextNode);
		times.t1[nextNode] = currentTime++;

		while (!nodesStack.empty())
		{
			while (expandNode(nodesStack, m_undirectedAdjacencyList, times.t1, currentTime, false));
			times.t2[nodesStack.top()] = currentTime++;
			nodesStack.pop();
		}

		m_connectedComponents.push_back(findComponent(times.t2, timeSnap, currentTime - 1));
		nextNodeIt = std::find(times.t2.begin(), times.t2.end(), 0);
	}
}

void Graph::generateUndirectedGraph()
{
	std::copy(m_adjacencyList.begin(), m_adjacencyList.end(), std::back_inserter(m_undirectedAdjacencyList));
	for (uint16_t lineIndex = 0; lineIndex < m_adjacencyList.size(); lineIndex++)
	{
		for (uint16_t nodeIndex = 0; nodeIndex < m_adjacencyList[lineIndex].size(); nodeIndex++)
		{
			if (std::find(m_undirectedAdjacencyList[m_adjacencyList[lineIndex][nodeIndex]].begin(),
				m_undirectedAdjacencyList[m_adjacencyList[lineIndex][nodeIndex]].end(), lineIndex)
				==
				m_undirectedAdjacencyList[m_adjacencyList[lineIndex][nodeIndex]].end())
			{
				m_undirectedAdjacencyList[m_adjacencyList[lineIndex][nodeIndex]].push_back(lineIndex);
			}
		}
	}
}

void Graph::dfsSCC()
{
	if (m_transposedAdjacencyList.empty())
		return;

	std::vector<uint16_t> visitedTimes(m_transposedAdjacencyList.size());
	uint16_t currentTime = 1;

	for (auto it = m_topologicalOrder.begin(); it != m_topologicalOrder.end(); ++it)
	{
		if (!visitedTimes[*it])
		{
			const uint16_t timeSnap = currentTime;
			visitedTimes[*it] = currentTime++;
			std::stack<uint16_t> nodesStack;
			nodesStack.push(*it);

			while (!nodesStack.empty())
			{
				while (expandNode(nodesStack, m_transposedAdjacencyList, visitedTimes, currentTime, false));
				visitedTimes[nodesStack.top()] = currentTime++;
				nodesStack.pop();
			}
			m_stronglyConnectedComponents.push_back(findComponent(visitedTimes, timeSnap, currentTime - 1));
		}
	}
}

void Graph::topologicalSort()
{
	auto sortedT2 = m_DFS.t2;
	std::sort(sortedT2.begin(), sortedT2.end());
	std::reverse(sortedT2.begin(), sortedT2.end());

	for (const auto& time : sortedT2)
	{
		auto foundAt = std::find(m_DFS.t2.begin(), m_DFS.t2.end(), time);
		uint16_t node = std::distance(m_DFS.t2.begin(), foundAt);
		m_topologicalOrder.push_back(node);
	}
}

void Graph::generateTransposedGraph()
{
	m_transposedAdjacencyList.resize(m_adjacencyList.size());
	for (uint16_t lineIndex = 0; lineIndex < m_adjacencyList.size(); lineIndex++)
	{
		for (uint16_t nodeIndex = 0; nodeIndex < m_adjacencyList[lineIndex].size(); nodeIndex++)
		{
			m_transposedAdjacencyList[m_adjacencyList[lineIndex][nodeIndex]].push_back(lineIndex);
		}
	}
}

bool Graph::hasCircuits() const
{
	for (uint16_t node = 0; node < m_adjacencyList.size(); node++)
		if (pathBetween(node, node))
			return true;
	return false;
}

std::vector<uint16_t> Graph::findComponent(const std::vector<uint16_t>& times, uint16_t fromTime, uint16_t toTime)
{
	std::vector<uint16_t> component;
	for (unsigned node = 0; node < times.size(); node++)
		if (times[node] >= fromTime && times[node] <= toTime)
			component.push_back(node);
	return component;
}

void Graph::generateSCCList()
{
	m_sccAdjacencyList.resize(m_stronglyConnectedComponents.size());

	for (unsigned fromIt = 0; fromIt < m_stronglyConnectedComponents.size() - 1; ++fromIt)
	{
		for (const auto& nodeFrom : m_stronglyConnectedComponents[fromIt])
		{
			bool isLinkerTo = false;
			bool isLinkerFrom = false;
			for (unsigned toIt = fromIt + 1; toIt < m_stronglyConnectedComponents.size() && (!isLinkerFrom || !isLinkerTo); ++toIt)
			{
				bool sameTo = false;
				bool sameFrom = false;
				for (const auto& nodeTo : m_stronglyConnectedComponents[toIt])
				{
					if (!sameTo && m_adjacencyMatrix[nodeFrom][nodeTo])
					{
						sameTo = true;
						isLinkerTo = true;
						m_sccAdjacencyList[fromIt].push_back(toIt);
					}
					if (!sameFrom && m_adjacencyMatrix[nodeTo][nodeFrom])
					{
						sameFrom = true;
						isLinkerFrom = true;
						m_sccAdjacencyList[toIt].push_back(fromIt);
					}
				}
			}
		}
	}
}

void Graph::generateMSTList()
{
	m_mstAdjacencyList.resize(m_wUndirectedAdjacencyMatrix.size());

	for (const auto& link : m_minimumSpanningTree)
	{
		m_mstAdjacencyList[link.first].push_back(link.second);
	}
}

std::string Graph::fromListToText(const std::vector<uint16_t>& list)
{
	std::stringstream text;
	for (const auto& node : list)
	{
		text << node << ",";
	}
	return text.str().substr(0, text.str().size() - 1);
}

std::string Graph::fromTopoToText(uint16_t nodeID)
{
	std::stringstream sStream;
	for (unsigned i = 0; i < m_topologicalOrder.size(); i++)
	{
		if (m_topologicalOrder[i] == nodeID)
		{
			sStream << i;
			return sStream.str();
		}
	}
	return "ERROR";
}
