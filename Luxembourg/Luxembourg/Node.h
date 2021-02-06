#pragma once
#include <string>

#include <SFML/Graphics.hpp>

inline float g_radius = 1.0f;

class Node :public sf::CircleShape
{
public:
	// Constr.
	Node(uint32_t id, float x = 0, float y = 0, sf::Color color = sf::Color::Red, float radius = g_radius);

	// GUI
	void drawNode(sf::RenderWindow& window) const;
	bool isTooClose(float x, float y) const;

	// Sort
	bool operator<(const Node& other) const;

	// G&S
	uint32_t GetID() const;
	void SetID(uint32_t id);

private:
	uint32_t m_id;
};

