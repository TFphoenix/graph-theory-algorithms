#pragma once
#include <SFML/Graphics.hpp>

class Arch
{
public:
	// Constr.
	Arch(uint16_t id, const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color = sf::Color::White);
	Arch(uint16_t id, const std::string& text, const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color = sf::Color::White);
	Arch(uint16_t id, const sf::Vector2f& position, sf::Color color = sf::Color::White);

	// GUI
	void drawArchLine(sf::RenderWindow& window) const;
	void drawArchArrowHead(sf::RenderWindow& window) const;

private:
	float arrowHeadRotation() const;
	sf::Vector2f arrowHeadPosition() const;

private:
	const inline static float s_arrowHeadRadius = 7.0f;
	const inline static int s_arrowHeadPointCount = 4;
	const inline static float s_arrowHeadDistance = 8.5f / 10.0f;
	const inline static float s_outlineThickness = 1.0f;
	const inline static float s_PI = 3.14159265f;

private:
	uint16_t m_ID;
	sf::Vector2f m_startPosition, m_endPosition, m_samePosition;
	sf::CircleShape m_arrowHead;
	sf::CircleShape m_circleLinker;
	sf::VertexArray m_line;
	float m_lineSlope;
	sf::Color m_color;
	std::string m_text;
};

