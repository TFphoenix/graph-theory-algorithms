#pragma once
#include <SFML/Graphics.hpp>

#include <string>

inline float g_radius = 20.0f;

class Node
{
public:
	Node(uint16_t id, float x = 0, float y = 0, sf::Color color = sf::Color::Red, float radius = g_radius);
	Node(uint16_t id, const std::string& text, float x = 0, float y = 0, sf::Color color = sf::Color::Red, float radius = g_radius);

	void drawNode(sf::RenderWindow& window) const;
	bool isTooClose(float x, float y) const;

	uint16_t GetID() const;
	void SetID(uint16_t id);
	sf::Vector2f GetPosition() const;
	void SetPosition(float x, float y);
	sf::Color GetFillColor() const;
	void SetFillColor(sf::Color color);
	sf::Color GetOutlineColor() const;
	void SetOutlineColor(sf::Color color);

private:
	static inline float s_outlineThickness = 3.0f;

private:
	uint16_t m_ID;
	sf::CircleShape m_body;
	sf::Vector2f m_position;
	sf::Color m_fillColor, m_outlineColor;
	std::string m_text;
};

