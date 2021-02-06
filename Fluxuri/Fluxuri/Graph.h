#pragma once
#include <string>
#include <vector>
#include <optional>

#include <SFML/Graphics.hpp>

#include "Node.h"
#include "Arch.h"

class Graph
{
public:
	enum class ReadStyle
	{
		FC,
		FCB
	};

	struct ArchProperties
	{
		ArchProperties(int f = 0, int c = 0, int b = 0, int r = 0) :f(f), c(c), b(b), r(r) {};

		bool operator<(const ArchProperties& other) const { return this->b < other.b; }

		int f;// flux
		int c;// capacitate
		int b;// cost de transport
		int r;// capacitate reziduala
	};
	using ArchMatrix = std::vector<std::vector<ArchProperties>>;
	using ListT = std::vector<std::vector<uint16_t>>;

public:
	// Constr.
	Graph(ReadStyle readStyle, const std::string& fileName);

	// G&S
	ReadStyle GetReadStyle() const;

	// Fluxuri
	void maximumFlowAlgorithm();
	void minimumCostFlowAlgorithm();
	uint16_t totalCost() const;

	// GUI
	void prepareGraphForDraw(const sf::RenderWindow& window);
	void drawGraph(sf::RenderWindow& window);

private:
	// Read & Initialize
	void readGraphFC();
	void readGraphFCB();
	void initializeNodes(float windowWidth, float windowHeight);
	void initializeArches();

	// Fluxuri
	void calculateResidualValues();
	std::optional<std::vector<uint16_t>> pathBetween(uint16_t source, uint16_t target);
	uint16_t residualMin(const std::vector<uint16_t>& path);
	void updateResidualValues(const std::vector<uint16_t>& path, uint16_t rMin);
	void maximizeFlow();
	void highlightMinimalCut();
	std::optional<std::vector<uint16_t>> negativeCycle();
	void minimizeFlow();
	std::vector<uint16_t> buildNegativeCycle(uint16_t startNode);

private:
	// Graph
	std::string m_inputFileName;
	ArchMatrix m_adjacencyMatrix;
	ListT m_adjacencyList;
	std::vector<Node> m_nodeList;
	std::vector<Arch> m_archList;
	ReadStyle m_readStyle;

	// Fluxuri
	uint16_t m_sourceNode;
	uint16_t m_targetNode;
	uint16_t m_value;
};

