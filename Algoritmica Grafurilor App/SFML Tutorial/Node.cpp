#include "Node.h"

Node::Node(uint16_t id, float x, float y, sf::Color color, float radius) :m_ID(id), m_position({ x, y }), m_fillColor(color)
{
	g_radius = radius;
	m_body.setRadius(radius);
	m_body.setOrigin(m_body.getRadius(), m_body.getRadius());
	m_body.setPosition(x, y);
	m_body.setFillColor(color);
}

Node::Node(uint16_t id, const std::string& text, float x, float y, sf::Color color, float radius) :m_ID(id), m_text(text), m_position({ x, y }), m_fillColor(color)
{
	g_radius = radius;
	m_body.setRadius(radius);
	m_body.setOrigin(m_body.getRadius(), m_body.getRadius());
	m_body.setPosition(x, y);
	m_body.setFillColor(color);
}

void Node::drawNode(sf::RenderWindow& window) const
{
	// Font
	sf::Font font;
	font.loadFromFile("font.ttf");

	// Text
	const std::string text = m_text.empty() ? std::to_string(m_ID) : m_text;
	sf::Text nodeText(text, font);
	nodeText.setFillColor(sf::Color::White);
	nodeText.setCharacterSize(20);
	nodeText.setStyle(sf::Text::Style::Bold);
	const sf::FloatRect bounds = nodeText.getLocalBounds();
	nodeText.setOrigin(m_body.getRadius() / (g_radius / 2.0f) + bounds.width / 2, m_body.getRadius() / (g_radius / 12.0f));
	nodeText.setPosition(m_position.x, m_position.y);

	// Draw
	window.draw(m_body);
	window.draw(nodeText);
}

bool Node::isTooClose(float x, float y) const
{
	float dx = m_position.x - x;
	float dy = m_position.y - y;
	const float euclideanDistance = std::sqrt(dx * dx + dy * dy);

	if (euclideanDistance < g_radius * 6)
		return true;
	return false;
}

uint16_t Node::GetID() const
{
	return m_ID;
}

void Node::SetID(uint16_t id)
{
	m_ID = id;
}

sf::Vector2f Node::GetPosition() const
{
	return m_position;
}

void Node::SetPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;
	m_body.setPosition(x, y);
}

sf::Color Node::GetFillColor() const
{
	return m_fillColor;
}

void Node::SetFillColor(sf::Color color)
{
	m_fillColor = color;
	m_body.setFillColor(color);
}

sf::Color Node::GetOutlineColor() const
{
	return m_outlineColor;
}

void Node::SetOutlineColor(sf::Color color)
{
	m_outlineColor = color;
	m_body.setOutlineColor(color);
	m_body.setOutlineThickness(s_outlineThickness);
}
