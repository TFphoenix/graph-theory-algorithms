#pragma once
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <utility>

#include <SFML/Graphics.hpp>

#include "Node.h"
#include "NodeLinker.h"

class Graph
{
public:
	enum class ReadStyle
	{
		Unknown,
		AdjacencyList,
		AdjacencyMatrix,
		Undirected
	};
	struct DFS_Result
	{
		bool empty();

		std::vector<uint16_t> t1;
		std::vector<uint16_t> t2;
	};
	struct Link
	{
		Link() = default;
		Link(uint16_t initialCost, uint16_t initialX, uint16_t initialY) :cost(initialCost), x(initialX), y(initialY) {};

		bool operator<(const Link& other) const { return this->cost < other.cost; }

		uint16_t cost;
		uint16_t x;
		uint16_t y;
	};
	using ListT = std::vector < std::vector < uint16_t >>;
	using LinkListT = std::vector<Link>;

private:
	enum class NodeInit
	{
		Classic,
		SCC,
		Topo,
		Undirected,
		MST
	};

public:
	Graph(ReadStyle readStyle, const std::string& fileName);
	void prepareGraphForDraw(const sf::RenderWindow& window);
	void prepareTopoForDraw(const sf::RenderWindow& window);
	void prepareSCCForDraw(const sf::RenderWindow& window);
	void prepareUndirectedGraphForDraw(const sf::RenderWindow& window);
	void prepareMSTForDraw(const sf::RenderWindow& window);
	void drawGraph(sf::RenderWindow& window);
	void drawTopo(sf::RenderWindow& window);
	void drawSCC(sf::RenderWindow& window);
	void drawUndirectedGraph(sf::RenderWindow& window);
	void drawMST(sf::RenderWindow& window);
	bool GetHasCircuits() const;
	std::vector<uint16_t> GetTopologicalOrder() const;
	ListT GetConnectedComponents() const;
	ListT GetStronglyConnectedComponents() const;
	void PrimAlgorithm();
	void KruskalAlgorithm();

private:
	void readGraphAsList();
	void readGraphAsMatrix();
	void readGraphUndirected();
	void fromListToMatrix();
	void fromMatrixToList();
	void fromWUMatrixToList();
	void fromWUMatrixToLinkerList();
	void fromLinkerListToMST(const LinkListT& linkerList);
	void initializeNodes(float windowWidth, float windowHeight, const ListT& adjacencyList, std::vector<Node>& nodeList, NodeInit nodeInit = NodeInit::Classic);
	void initializeLinkers(const ListT& adjacencyList, std::vector<Node>& nodeList, std::vector<NodeLinker>& nodeLinkerList);
	static bool expandNode(std::stack<uint16_t>& container, const ListT& adjacencyList, std::vector<uint16_t>& visitedTimes, uint16_t& startTime, bool deleteTop = false);
	static bool expandNode(std::queue<uint16_t>& container, const ListT& adjacencyList, std::vector<uint16_t>& visitedTimes, uint16_t& startTime, bool deleteTop = false);
	bool pathBetween(uint16_t from, uint16_t to) const;
	DFS_Result dfsClassic();
	void dfsCC();
	void dfsSCC();
	void topologicalSort();
	void generateTransposedGraph();
	void generateUndirectedGraph();
	bool hasCircuits() const;
	static std::vector<uint16_t> findComponent(const std::vector<uint16_t>& times, uint16_t fromTime, uint16_t toTime);
	void generateSCCList();
	void generateMSTList();
	static std::string fromListToText(const std::vector<uint16_t>& list);
	std::string fromTopoToText(uint16_t nodeID);

private:
	// basic
	std::string m_inputFileName;
	ListT m_adjacencyList;
	std::vector<std::vector<bool>> m_adjacencyMatrix;
	std::vector<Node> m_nodeList;
	std::vector<NodeLinker> m_nodeLinkerList;
	// topo, cc & scc
	bool m_hasCircuits;
	std::vector<uint16_t> m_topologicalOrder;
	ListT m_transposedAdjacencyList, m_undirectedAdjacencyList;
	DFS_Result m_DFS;
	ListT m_connectedComponents, m_stronglyConnectedComponents;
	ListT m_sccAdjacencyList;
	std::vector<Node> m_sccNodeList;
	std::vector<NodeLinker> m_sccNodeLinkerList;
	// weighted undirected
	bool m_isWUndirected;
	std::vector<std::vector<int16_t>> m_wUndirectedAdjacencyMatrix;
	LinkListT m_wULinkerList;
	std::vector<std::pair<uint16_t, uint16_t>> m_minimumSpanningTree;
	ListT m_mstAdjacencyList;
	std::vector<Node> m_undirectedNodeList;
	std::vector<NodeLinker> m_undirectedNodeLinkerList, m_mstNodeLinkerList;
};
