#pragma once
#include "Labyrinth.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

class LabyrinthManager
{
public:
	LabyrinthManager(const std::string& inputFilename, const std::string& outputFilename);
	void solveLabyrinth();
	void initializeDraw(sf::RenderWindow& window);
	void drawLabyrinth(sf::RenderWindow& window);

private:
	void drawLabyrinthBody(sf::RenderWindow& window);
	void findExitPoints();
	void determinePaths();
	struct stringComparator {
		bool operator()(const std::string& first, const std::string& second) const;
	};

private:
	static const size_t s_windowBordersSize = 60;

private:
	Labyrinth* m_labyrinth;
	size_t m_sizeL, m_sizeC;
	coord2D m_startPoint;
	std::vector<coord2D> m_exitPoints;
	std::vector <std::string> m_paths;
	std::string m_InFile, m_OutFile;
	std::vector<std::vector<sf::RectangleShape>> m_cellsBody;
};