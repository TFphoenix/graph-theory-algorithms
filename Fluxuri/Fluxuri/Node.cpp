#include "Node.h"

Node::Node(uint16_t id, float x, float y, sf::Color color, float radius) :m_id(id)
{
	g_radius = radius;
	setRadius(radius);
	setOrigin(getRadius(), getRadius());
	setPosition(x, y);
	setFillColor(color);
}

Node::Node(uint16_t id, const std::string& text, float x, float y, sf::Color color, float radius) :m_id(id), m_text(text)
{
	g_radius = radius;
	setRadius(radius);
	setOrigin(getRadius(), getRadius());
	setPosition(x, y);
	setFillColor(color);
}

void Node::drawNode(sf::RenderWindow& window) const
{
	// Font
	sf::Font font;
	font.loadFromFile("font.ttf");

	// Text
	const std::string text = m_text.empty() ? std::to_string(m_id) : m_text;
	sf::Text nodeText(text, font);
	nodeText.setFillColor(sf::Color::White);
	nodeText.setCharacterSize(20);
	nodeText.setStyle(sf::Text::Style::Bold);
	const sf::FloatRect bounds = nodeText.getLocalBounds();
	nodeText.setOrigin(getRadius() / (g_radius / 2.0f) + bounds.width / 2, getRadius() / (g_radius / 12.0f));
	nodeText.setPosition(getPosition().x, getPosition().y);

	// Draw
	window.draw(*dynamic_cast<sf::CircleShape*>(const_cast<Node*>(this)));
	window.draw(nodeText);
}

bool Node::isTooClose(float x, float y) const
{
	float dx = getPosition().x - x;
	float dy = getPosition().y - y;
	const float euclideanDistance = std::sqrt(dx * dx + dy * dy);

	if (euclideanDistance < g_radius * 6)
		return true;
	return false;
}

uint16_t Node::GetID() const
{
	return m_id;
}

void Node::SetID(uint16_t id)
{
	m_id = id;
}
