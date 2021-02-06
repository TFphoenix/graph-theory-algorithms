#include "NodeLinker.h"
#include "Node.h"

NodeLinker::NodeLinker(uint16_t id, const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color) :m_ID(id), m_startPosition(start), m_endPosition(end), m_color(color)
{
	m_lineSlope = (m_endPosition.y - m_startPosition.y) / (m_endPosition.x - m_startPosition.x);

	m_arrowHead.setRadius(s_arrowHeadRadius);
	m_arrowHead.setPointCount(s_arrowHeadPointCount);
	m_arrowHead.setOrigin(m_arrowHead.getRadius(), m_arrowHead.getRadius());
	m_arrowHead.setFillColor(m_color);
	m_line[0] = sf::Vertex(m_startPosition, m_color);
	m_line[1] = sf::Vertex(m_endPosition, m_color);

	m_arrowHead.setPosition(arrowHeadPosition());
	m_arrowHead.rotate(arrowHeadRotation());
	m_line[0] = m_startPosition;
	m_line[1] = m_endPosition;
}

NodeLinker::NodeLinker(uint16_t id, const sf::Vector2f& position, sf::Color color)
	:m_ID(id), m_samePosition(position), m_color(color)
{
	m_circleLinker.setFillColor(sf::Color::Transparent);
	m_circleLinker.setOutlineColor(sf::Color::White);
	m_circleLinker.setOutlineThickness(s_outlineThickness);
	m_circleLinker.setOrigin(m_circleLinker.getRadius()+25, m_circleLinker.getRadius()+25);
	m_circleLinker.setPosition(m_samePosition);
	m_circleLinker.setRadius(g_radius + 5.0f);
}

void NodeLinker::drawLinkerLine(sf::RenderWindow& window) const
{
	if (m_lineSlope != 0.0f)
		window.draw(m_line, 2, sf::Lines);
	else
		window.draw(m_circleLinker);
}

void NodeLinker::drawLinkerArrowHead(sf::RenderWindow& window) const
{
	window.draw(m_arrowHead);
}

float NodeLinker::arrowHeadRotation() const
{
	return atan(m_lineSlope) * 180 / s_PI;
}

sf::Vector2f NodeLinker::arrowHeadPosition() const
{
	const float x_dif = m_endPosition.x - m_startPosition.x;
	const float y_dif = m_endPosition.y - m_startPosition.y;
	const float dx = m_startPosition.x - m_endPosition.x;
	const float dy = m_startPosition.y - m_endPosition.y;
	const float euclideanDistance = std::sqrt(dx * dx + dy * dy);

	const float x = m_startPosition.x + x_dif * s_arrowHeadDistance;
	const float y = m_startPosition.y + y_dif * s_arrowHeadDistance;
	//arrowHead on head of linker
	/*const float x = m_startPosition.x + x_dif * ((euclideanDistance - g_radius) / euclideanDistance);
	const float y = m_startPosition.y + y_dif * ((euclideanDistance - g_radius) / euclideanDistance);*/
	return sf::Vector2f(x, y);
}
