#include "LabyrinthManager.h"

#include <SFML/System/Time.hpp>

#include <fstream>

LabyrinthManager::LabyrinthManager(const std::string& inputFilename,
	const std::string& outputFilename) :m_InFile(inputFilename), m_OutFile(outputFilename)
{
	m_labyrinth = new Labyrinth(m_InFile);
	m_startPoint = m_labyrinth->GetFrom();
}

void LabyrinthManager::solveLabyrinth()
{
	findExitPoints();
	determinePaths();
}

void LabyrinthManager::initializeDraw(sf::RenderWindow& window)
{
	const auto matrix = m_labyrinth->GetMatrix();
	m_sizeL = m_labyrinth->GetSizeL();
	m_sizeC = m_labyrinth->GetSizeC();
	size_t windowSize;
	size_t biggestSize;
	if (m_sizeL > m_sizeC)
	{
		biggestSize = m_sizeL;
		windowSize = window.getSize().y;
	}
	else
	{
		biggestSize = m_sizeC;
		windowSize = window.getSize().x;
	}
	const auto cellSize = static_cast<float>((windowSize - s_windowBordersSize) / biggestSize);

	std::vector<std::vector<sf::RectangleShape>> cells(m_sizeL, std::vector<sf::RectangleShape>(m_sizeC, sf::RectangleShape(sf::Vector2f(cellSize, cellSize))));
	for (size_t line = 0; line < m_sizeL; line++)
	{
		for (size_t column = 0; column < m_sizeC; column++)
		{
			cells[line][column].setPosition(column * cellSize, line * cellSize);
			if (matrix[line][column] == WALL)
				cells[line][column].setFillColor(sf::Color::Black);
			else if (matrix[line][column] == FREE)
				cells[line][column].setFillColor(sf::Color::White);
			else
				cells[line][column].setFillColor(sf::Color::Yellow);
		}
	}
	m_cellsBody = cells;

	const sf::View view(sf::Vector2f((m_sizeC * cellSize) / 2, (m_sizeL * cellSize) / 2), sf::Vector2f(window.getSize().x, window.getSize().y));
	window.setView(view);
}

void LabyrinthManager::drawLabyrinth(sf::RenderWindow& window)
{
	const sf::Time sleepTime = sf::seconds(0.2f);
	const sf::Time initialSleepTime = sf::seconds(1.0f);
	drawLabyrinthBody(window);
	window.display();
	sf::sleep(initialSleepTime);

	sf::Texture pointerTexture;
	pointerTexture.loadFromFile("MyFace_PNG.png");
	const auto pointerRadius = m_cellsBody[0][0].getSize().x / 2;
	const auto fromCoords = m_labyrinth->GetFrom();
	sf::CircleShape pointer(pointerRadius);
	pointer.setTexture(&pointerTexture);
	for (const auto& path : m_paths)
	{
		auto currentPosition = fromCoords;
		pointer.setPosition(m_cellsBody[fromCoords.x][fromCoords.y].getPosition());

		for (const auto& direction : path)
		{
			switch (direction)
			{
			case 'n':
				break;
			case 'u':
				currentPosition.x -= 1;
				break;
			case 'r':
				currentPosition.y += 1;
				break;
			case 'd':
				currentPosition.x += 1;
				break;
			case 'l':
				currentPosition.y -= 1;
				break;
			default:
				break;
			}
			m_cellsBody[currentPosition.x][currentPosition.y].setFillColor(sf::Color::Red);
			pointer.setPosition(m_cellsBody[currentPosition.x][currentPosition.y].getPosition());
			drawLabyrinthBody(window);
			window.draw(pointer);
			window.display();
			sf::sleep(sleepTime);
		}
	}
	drawLabyrinthBody(window);
	m_cellsBody[m_startPoint.x][m_startPoint.y].setFillColor(sf::Color::Green);
	pointer.setPosition(m_cellsBody[fromCoords.x][fromCoords.y].getPosition());
	window.draw(m_cellsBody[m_startPoint.x][m_startPoint.y]);
	window.draw(pointer);
	window.display();
}

void LabyrinthManager::drawLabyrinthBody(sf::RenderWindow& window)
{
	window.clear(sf::Color(53, 53, 133));
	for (size_t line = 0; line < m_sizeL; line++)
	{
		for (size_t column = 0; column < m_sizeC; column++)
		{
			window.draw(m_cellsBody[line][column]);
		}
	}
	m_cellsBody[m_startPoint.x][m_startPoint.y].setFillColor(sf::Color::Yellow);
	window.draw(m_cellsBody[m_startPoint.x][m_startPoint.y]);
}

void LabyrinthManager::findExitPoints()
{
	if (m_labyrinth)
	{
		auto matrix = m_labyrinth->GetMatrix();
		const auto sizeL = m_labyrinth->GetSizeL();
		const auto sizeC = m_labyrinth->GetSizeC();
		for (size_t i = 0; i < sizeL; i++)
			for (size_t j = 0; j < sizeC; j++)
				if (i == 0 || j == 0 || i == sizeL - 1 || j == sizeC - 1)
					if (matrix[i][j] == 1)
						m_exitPoints.emplace_back(coord2D(i, j));
	}
}

void LabyrinthManager::determinePaths()
{
	for (auto& exitPoint : m_exitPoints)
	{
		m_labyrinth->SetTo(exitPoint);
		m_labyrinth->solve_maze_Astar(m_OutFile, false);

		std::ifstream pathFile(m_OutFile);
		std::string path;
		std::getline(pathFile, path);
		if (!path.empty())
			if (path[0] != '@')
				m_paths.push_back(path);
	}
	const stringComparator comparator;
	std::sort(m_paths.begin(), m_paths.end(), comparator);
}

bool LabyrinthManager::stringComparator::operator()(const std::string& first, const std::string& second) const
{
	return first.size() < second.size();
}
