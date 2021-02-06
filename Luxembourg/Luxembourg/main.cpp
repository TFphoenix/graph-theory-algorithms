#include <SFML/Graphics.hpp>

#include "Graph.h"
#include "Timer.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(621, 887), "Luxembourg", sf::Style::Titlebar | sf::Style::Close);
	Timer timer;
	timer.start();
	Graph graph("map2.xml", window.getSize());
	timer.stop();
	std::cout << "Citire si initializare: " << timer.elapsedMilliseconds() / 1000.0f << " [secunde]\n";

	bool firstClick = true;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				if (firstClick)
				{
					timer.start();
					graph.setSourcePoint(event.mouseButton.x, event.mouseButton.y);
					firstClick = false;
					timer.stop();
					std::cout << "Primul punct setat in: " << timer.elapsedMilliseconds() / 1000.0f << " [secunde]\n";
				}
				else
				{
					timer.start();
					graph.setTargetPointAndApplyDijkstra(event.mouseButton.x, event.mouseButton.y);
					timer.stop();
					std::cout << "Drum minim (Dijkstra) gasit in: " << timer.elapsedMilliseconds() / 1000.0f << " [secunde]\n";
				}
			}
		}

		window.clear();
		graph.drawGraph(window);
		window.display();
	}

	return 0;
}