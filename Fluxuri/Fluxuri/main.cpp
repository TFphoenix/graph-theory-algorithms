#include <SFML/Graphics.hpp>
#include "UIButton.h"
#include "Graph.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Fluxuri");
	UIButton flowButton1("FLUX MAXIM");
	UIButton flowButton2("FLUX COST MINIM", sf::Vector2f(170, 10));
	UIButton calculateButton("CALCULEAZA", sf::Vector2f(330, 10));
	UIButton beforeText("", sf::Vector2f(680, 10), sf::Vector2f(50, 50));
	UIButton afterText("", sf::Vector2f(740, 10), sf::Vector2f(50, 50));
	calculateButton.ChangeState(UIButton::State::Disabled);

	Graph* graf = nullptr;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			flowButton1.HandleEvent(event);
			flowButton2.HandleEvent(event);
			calculateButton.HandleEvent(event);

			if (flowButton1.GetState() == UIButton::State::Release)
			{
				beforeText.ChangeText("");
				afterText.ChangeText("");

				flowButton1.ChangeState(UIButton::State::Hover);
				calculateButton.ChangeState(UIButton::State::None);
				delete graf;
				graf = new Graph(Graph::ReadStyle::FC, "FluxMaxim.in");
				graf->prepareGraphForDraw(window);
			}

			if (flowButton2.GetState() == UIButton::State::Release)
			{
				flowButton2.ChangeState(UIButton::State::Hover);
				calculateButton.ChangeState(UIButton::State::None);
				delete graf;
				graf = new Graph(Graph::ReadStyle::FCB, "FluxCostMinim.in");
				graf->prepareGraphForDraw(window);
				beforeText.ChangeText(std::to_string(graf->totalCost()));
			}

			if (calculateButton.GetState() == UIButton::State::Release)
			{
				calculateButton.ChangeState(UIButton::State::Disabled);
				switch (graf->GetReadStyle())
				{
				case Graph::ReadStyle::FC:
					graf->maximumFlowAlgorithm();
					break;
				case Graph::ReadStyle::FCB:
					graf->minimumCostFlowAlgorithm();
					afterText.ChangeText(std::to_string(graf->totalCost()));
					break;
				default:
					break;
				}
			}
		}

		window.clear();
		if (graf)
			graf->drawGraph(window);
		flowButton1.draw(window);
		flowButton2.draw(window);
		calculateButton.draw(window);
		beforeText.draw(window);
		afterText.draw(window);
		window.display();
	}

	return 0;
}
