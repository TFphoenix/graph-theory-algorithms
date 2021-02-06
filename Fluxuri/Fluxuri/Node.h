#pragma once
#include <string>

#include <SFML/Graphics.hpp>

inline float g_radius = 20.0f;

class Node :public sf::CircleShape
{
public:
	// Constr.
	Node(uint16_t id, float x = 0, float y = 0, sf::Color color = sf::Color::Red, float radius = g_radius);
	Node(uint16_t id, const std::string& text, float x = 0, float y = 0, sf::Color color = sf::Color::Red, float radius = g_radius);

	// GUI
	void drawNode(sf::RenderWindow& window) const;
	bool isTooClose(float x, float y) const;

	// G&S
	uint16_t GetID() const;
	void SetID(uint16_t id);

private:
	static inline float s_outlineThickness = 3.0f;

private:
	uint16_t m_id;
	std::string m_text;
};

