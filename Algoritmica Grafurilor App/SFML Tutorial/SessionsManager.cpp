#include "SessionsManager.h"
#include "Graph.h"
#include "LabyrinthManager.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <conio.h>

SessionsManager::SessionsManager()
{
	MainSession();
}

void SessionsManager::MainSession()
{
	unsigned option = 0;
	system("CLS");
	std::cout << "Sessions:\n" <<
		"1 - Tema1: Reprezentare graf\n" <<
		"2 - Tema2: Labirint\n" <<
		"3 - Tema3: Sortare topologica, componente conexe, componente tare conexe\n" <<
		"4 - Tema4: Arbore partial minim\n" <<
		"Other - Exit\n\n" <<
		"Choose your session: ";
	std::cin >> option;

	switch (option)
	{
	case 1:
		std::cout << "Starting \"Tema 1: Reprezentare graf\"...\n";
		Tema1Session();
		break;
	case 2:
		std::cout << "Starting \"Tema 2: Labirint\"...\n";
		Tema2Session();
		break;
	case 3:
		std::cout << "Starting \"Tema 3: Sortare topologica, componente conexe, componente tare conexe\"...\n";
		Tema3Session();
		break;
	case 4:
		std::cout << "Starting \"Tema4: Arbore partial minim\"...\n";
		Tema4Session();
		break;
	default:
		break;
	}
}

void SessionsManager::Tema1Session()
{
	sf::RenderWindow window(sf::VideoMode(s_windowWidth, s_windowHeight), "Reprezentare grafica graf", sf::Style::Close | sf::Style::Titlebar);
	Graph graph(Graph::ReadStyle::AdjacencyList, "AdjacencyList.in");
	graph.prepareGraphForDraw(window);

	while (window.isOpen())
	{
		sf::Event eventSystem;
		while (window.pollEvent(eventSystem))
		{
			if (eventSystem.type == sf::Event::Closed)
			{
				window.close();
				MainSession();
			}
		}

		window.clear();
		graph.drawGraph(window);
		window.display();
	}
}

void SessionsManager::Tema2Session()
{
	sf::RenderWindow window(sf::VideoMode(s_windowWidth, s_windowHeight), "Labirint", sf::Style::Close | sf::Style::Titlebar);

	LabyrinthManager labyrinthManager("Labyrinth.in", "Labyrinth.out");
	labyrinthManager.solveLabyrinth();
	labyrinthManager.initializeDraw(window);
	labyrinthManager.drawLabyrinth(window);

	while (window.isOpen())
	{
		sf::Event eventSystem;
		while (window.pollEvent(eventSystem))
		{
			if (eventSystem.type == sf::Event::Closed)
			{
				window.close();
				MainSession();
			}
		}
	}
}

void SessionsManager::Tema3Session()
{
	sf::RenderWindow sccWindow(sf::VideoMode(s_windowWidth, s_windowHeight), "Componente tare conexe", sf::Style::Close | sf::Style::Titlebar);
	sf::RenderWindow topoWindow(sf::VideoMode(s_windowWidth, s_windowHeight), "Sortare topologica", sf::Style::Close | sf::Style::Titlebar);
	Graph graph(Graph::ReadStyle::AdjacencyList, "AdjacencyList1.in");
	graph.prepareTopoForDraw(topoWindow);
	graph.prepareSCCForDraw(sccWindow);

	// Topo, conex, tare conex
	std::cout << "\n> Sortare topologica:\n";
	try {
		const auto topologicalOrder = graph.GetTopologicalOrder();
		for (const auto& node : topologicalOrder)
			std::cout << node << " ";
	}
	catch (const char* errorMessage)
	{
		std::cout << errorMessage;
	}
	std::cout << "\n> Componente conexe:\n";
	const auto connectedComponents = graph.GetConnectedComponents();
	uint16_t componentIndex = 1;
	for (const auto& line : connectedComponents)
	{
		std::cout << "CC" << componentIndex++ << ": ";
		for (const auto& node : line)
			std::cout << node << " ";
		std::cout << "\n";
	}
	std::cout << "> Componente tare conexe:\n";
	const auto stronglyConnectedComponents = graph.GetStronglyConnectedComponents();
	componentIndex = 1;
	for (const auto& line : stronglyConnectedComponents)
	{
		std::cout << "SCC" << componentIndex++ << ": ";
		for (const auto& node : line)
			std::cout << node << " ";
		std::cout << "\n";
	}

	// topo + tare conex - grafic
	while (sccWindow.isOpen() && topoWindow.isOpen())
	{
		sf::Event eventSystem;
		while (sccWindow.pollEvent(eventSystem))
		{
			if (eventSystem.type == sf::Event::Closed)
			{
				sccWindow.close();
				topoWindow.close();
				MainSession();
			}
		}
		while (topoWindow.pollEvent(eventSystem))
		{
			if (eventSystem.type == sf::Event::Closed)
			{
				topoWindow.close();
				sccWindow.close();
				MainSession();
			}
		}

		topoWindow.clear();
		graph.drawTopo(topoWindow);
		topoWindow.display();

		sccWindow.clear();
		graph.drawSCC(sccWindow);
		sccWindow.display();
	}
}

void SessionsManager::Tema4Session()
{
	sf::RenderWindow undirectedWindow(sf::VideoMode(s_windowWidth, s_windowHeight), "Graf neorientat", sf::Style::Close | sf::Style::Titlebar);
	sf::RenderWindow mstWindow(sf::VideoMode(s_windowWidth, s_windowHeight), "Arbore partial minim", sf::Style::Close | sf::Style::Titlebar);
	Graph undirectedGraph(Graph::ReadStyle::Undirected, "UndirectedGraph.in");
	undirectedGraph.prepareUndirectedGraphForDraw(undirectedWindow);
	//undirectedGraph.PrimAlgorithm();
	undirectedGraph.KruskalAlgorithm();
	undirectedGraph.prepareMSTForDraw(mstWindow);

	while (mstWindow.isOpen() && undirectedWindow.isOpen())
	{
		sf::Event eventSystem;
		while (mstWindow.pollEvent(eventSystem))
		{
			if (eventSystem.type == sf::Event::Closed)
			{
				mstWindow.close();
				undirectedWindow.close();
				MainSession();
			}
		}
		while (undirectedWindow.pollEvent(eventSystem))
		{
			if (eventSystem.type == sf::Event::Closed)
			{
				undirectedWindow.close();
				mstWindow.close();
				MainSession();
			}
		}

		undirectedWindow.clear();
		undirectedGraph.drawUndirectedGraph(undirectedWindow);
		undirectedWindow.display();

		mstWindow.clear();
		undirectedGraph.drawMST(mstWindow);
		mstWindow.display();
	}
}
