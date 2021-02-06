#include "Graph.h"
#include "Random.h"
#include "rapidxml.hpp"

#include <fstream>
#include <cmath>
#include <queue>

Graph::Graph(std::string&& fileName, const sf::Vector2u& windowSize) :
	m_inputFileName(fileName),
	m_windowSize(windowSize)
{
	// Logic
	readGraph();

	// GUI
	m_drawSourceNode = false;
	m_drawTargetNode = false;
	sf::CircleShape templateNodeGUI(5.0f);
	templateNodeGUI.setFillColor(sf::Color(255, 0, 0));
	templateNodeGUI.setOrigin(templateNodeGUI.getRadius(), templateNodeGUI.getRadius());
	m_sourceNodeGUI = templateNodeGUI;
	m_targetNodeGUI = templateNodeGUI;
}

void Graph::drawGraph(sf::RenderWindow& window)
{
	for (const auto& arch : m_archList)
		arch.drawArchLine(window);
	for (const auto& arch : m_path)
		arch.drawArchLine(window);
	if (m_drawSourceNode) window.draw(m_sourceNodeGUI);
	if (m_drawTargetNode) window.draw(m_targetNodeGUI);


	/*for (const auto& node : m_nodeList)
		node.drawNode(window);*/
		/*for (const auto& linker : m_archList)
			linker.drawArchArrowHead(window);*/
}

void Graph::setSourcePoint(int mouseX, int mouseY)
{
	m_sourceId = closestNodeId(mouseX, mouseY);
	m_sourceNodeGUI.setPosition(m_nodeList[m_sourceId].getPosition());
	m_drawSourceNode = true;
}

void Graph::setTargetPointAndApplyDijkstra(int mouseX, int mouseY)
{
	m_targetId = closestNodeId(mouseX, mouseY);
	m_targetNodeGUI.setPosition(m_nodeList[m_targetId].getPosition());
	m_drawTargetNode = true;
	DijkstraAlgorithm();
}

void Graph::readGraph()
{
	// Parse XML Document
	std::ifstream inputFile(m_inputFileName);
	std::string* fileContent = new std::string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
	inputFile.close();
	rapidxml::xml_document<> xmlDocument;
	xmlDocument.parse<0>(const_cast<char*>(fileContent->c_str()));

	// Read XML Document
	rapidxml::xml_node<>* xmlRoot = xmlDocument.first_node();
	rapidxml::xml_node<>* xmlNodes = xmlRoot->first_node("nodes");
	rapidxml::xml_node<>* xmlArches = xmlRoot->first_node("arcs");
	/// Nodes
	for (rapidxml::xml_node<>* xmlNode = xmlNodes->first_node(); xmlNode; xmlNode = xmlNode->next_sibling())
	{
		const uint32_t id = atoi(xmlNode->first_attribute("id")->value());
		const auto x = static_cast<float>(atoi(xmlNode->first_attribute("latitude")->value()));
		const auto y = static_cast<float>(atoi(xmlNode->first_attribute("longitude")->value()));

		m_dimensions.update(x, y);
		m_nodeList.emplace_back(id, x, y);
	}
	scaleNodePositions();
	/// Arcs
	uint32_t archId = 0;
	m_adjacencyList.resize(m_nodeList.size());
	for (rapidxml::xml_node<>* xmlArch = xmlArches->first_node(); xmlArch; xmlArch = xmlArch->next_sibling())
	{
		const auto from = static_cast<float>(atoi(xmlArch->first_attribute("from")->value()));
		const auto to = static_cast<float>(atoi(xmlArch->first_attribute("to")->value()));
		const auto length = static_cast<float>(atoi(xmlArch->first_attribute("length")->value()));

		m_archList.emplace_back(archId++, m_nodeList[from].getPosition(), m_nodeList[to].getPosition());
		m_adjacencyList[from].emplace_front(length, to);
	}

}

void Graph::scaleNodePositions()
{
	m_dimensions.yRatio = m_windowSize.y / (m_dimensions.maxY - m_dimensions.minY);
	m_dimensions.xRatio = m_windowSize.x / (m_dimensions.maxX - m_dimensions.minX);
	const auto ySize = (m_dimensions.maxY - m_dimensions.minY) * m_dimensions.yRatio;

	for (auto& node : m_nodeList)
	{
		node.setPosition((node.getPosition().x - m_dimensions.minX) * m_dimensions.xRatio, -(node.getPosition().y - m_dimensions.minY) * m_dimensions.yRatio + ySize);
	}
}

uint32_t Graph::closestNodeId(int mouseX, int mouseY)
{
	float minDistance = std::numeric_limits<float>::infinity();
	uint32_t id = 0;

	for (const auto& node : m_nodeList)
	{
		const float currentDistance = sqrt(static_cast<float>(mouseX) - node.getPosition().x)* sqrt(static_cast<float>(mouseX) - node.getPosition().x) + sqrt(static_cast<float>(mouseY) - node.getPosition().y)* sqrt(static_cast<float>(mouseY) - node.getPosition().y);
		if (currentDistance < minDistance)
		{
			minDistance = currentDistance;
			id = node.GetID();
		}
		else if (minDistance == currentDistance)
		{
			return node.GetID();
		}
	}

	return id;
}

void Graph::DijkstraAlgorithm()
{
	std::vector<bool> visited(m_adjacencyList.size(), false);
	std::vector<uint32_t> distances(m_adjacencyList.size(), std::numeric_limits<uint32_t>::infinity());
	std::vector<std::optional<uint32_t>> parents(m_adjacencyList.size(), std::nullopt);
	//std::priority_queue<ArchProperties, std::vector<ArchProperties>, std::greater<>> queue;
	std::queue<ArchProperties> queue;

	distances[m_sourceId] = 0;
	queue.emplace(distances[m_sourceId], m_sourceId);

	while (!queue.empty())
	{
		//const auto currentNode = queue.top();
		const auto currentNode = queue.front();
		queue.pop();
		if (visited[currentNode.nodeId]) continue;

		visited[currentNode.nodeId] = true;
		for (const auto& arch : m_adjacencyList[currentNode.nodeId])
		{
			if (visited[arch.nodeId] == false)
			{
				if (distances[arch.nodeId] > currentNode.cost + arch.cost || distances[arch.nodeId] == std::numeric_limits<uint32_t>::infinity())
				{
					distances[arch.nodeId] = currentNode.cost + arch.cost;
					parents[arch.nodeId].emplace(currentNode.nodeId);
					queue.emplace(arch.cost, arch.nodeId);
					/*if (currentNode.nodeId == m_targetId)
					{
						buildPath(std::move(parents));
						return;
					}*/
				}
			}
		}
	}

	buildPath(std::move(parents));
}

void Graph::buildPath(std::vector<std::optional<uint32_t>>&& parents)
{
	m_path.clear();
	auto currentId = m_targetId;

	uint32_t pathId = 0;
	while (parents[currentId].has_value())
	{
		m_path.emplace_back(pathId++, m_nodeList[currentId].getPosition(), m_nodeList[parents[currentId].value()].getPosition(), sf::Color::Red);
		currentId = parents[currentId].value();
	}
}