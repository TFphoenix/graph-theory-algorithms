#include "Node.h"

Node::Node(uint32_t id, float x, float y, sf::Color color, float radius) :m_id(id)
{
	g_radius = radius;
	setRadius(radius);
	setOrigin(getRadius(), getRadius());
	setPosition(x, y);
	setFillColor(color);
}

void Node::drawNode(sf::RenderWindow& window) const
{
	window.draw(*dynamic_cast<sf::CircleShape*>(const_cast<Node*>(this)));
}

bool Node::isTooClose(float x, float y) const
{
	const float dx = getPosition().x - x;
	const float dy = getPosition().y - y;
	const float euclideanDistance = std::sqrt(dx * dx + dy * dy);

	if (euclideanDistance < g_radius * 6)
		return true;
	return false;
}

bool Node::operator<(const Node& other) const
{
	return this->getPosition().y < other.getPosition().y;
}

uint32_t Node::GetID() const
{
	return m_id;
}

void Node::SetID(uint32_t id)
{
	m_id = id;
}
