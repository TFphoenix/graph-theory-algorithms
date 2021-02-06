#pragma once
#include <string>
#include <vector>
#include <list>
#include <limits>
#include <optional>

#include <SFML/Graphics.hpp>

#include "Node.h"
#include "Arch.h"

class Graph
{
public:
	// Adjacency List Type
	struct ArchProperties
	{
		ArchProperties(uint16_t cost = 0, uint32_t nodeId = 0) :cost(cost), nodeId(nodeId) {};

		bool operator<(const ArchProperties& other) const { return this->cost < other.cost; }
		bool operator>(const ArchProperties& other) const { return this->cost > other.cost; }

		uint16_t cost;
		uint32_t nodeId;
	};
	// Graph Dimensions Type
	struct Dimensions
	{
		void update(float x, float y)
		{
			minX = x < minX ? x : minX;
			maxX = x > maxX ? x : maxX;
			minY = y < minY ? y : minY;
			maxY = y > maxY ? y : maxY;
		}

		float minX = std::numeric_limits<float>::infinity();
		float maxX = -std::numeric_limits<float>::infinity();
		float minY = std::numeric_limits<float>::infinity();
		float maxY = -std::numeric_limits<float>::infinity();
		float xRatio = 1.0f;
		float yRatio = 1.0f;
	};
	using ListT = std::vector<std::list<ArchProperties>>;

public:
	// Constr.
	Graph(std::string&& fileName, const sf::Vector2u& windowSize);

	// GUI
	void drawGraph(sf::RenderWindow& window);

	// Luxembourg
	void setSourcePoint(int mouseX, int mouseY);
	void setTargetPointAndApplyDijkstra(int mouseX, int mouseY);

private:
	// Read & Initialize
	void readGraph();
	void scaleNodePositions();

	// Luxembourg
	uint32_t closestNodeId(int mouseX, int mouseY);
	void DijkstraAlgorithm();
	void buildPath(std::vector<std::optional<uint32_t>>&& parents);

private:
	// Graph
	std::string m_inputFileName;
	sf::Vector2u m_windowSize;
	ListT m_adjacencyList;
	std::vector<Node> m_nodeList;
	std::vector<Arch> m_archList;
	Dimensions m_dimensions;

	// Luxembourg
	uint32_t m_sourceId;
	uint32_t m_targetId;
	bool m_drawSourceNode;
	bool m_drawTargetNode;
	sf::CircleShape m_sourceNodeGUI;
	sf::CircleShape m_targetNodeGUI;
	std::vector<Arch> m_path;
};

