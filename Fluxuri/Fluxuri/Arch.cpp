#include "Arch.h"
#include "Node.h"

Arch::Arch(uint16_t id, const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color) :
	m_line(sf::Lines, 2), m_ID(id), m_startPosition(start), m_endPosition(end), m_color(color)
{
	m_lineSlope = (m_endPosition.y - m_startPosition.y) / (m_endPosition.x - m_startPosition.x);

	m_arrowHead.setRadius(s_arrowHeadRadius);
	m_arrowHead.setPointCount(s_arrowHeadPointCount);
	m_arrowHead.setOrigin(m_arrowHead.getRadius(), m_arrowHead.getRadius());
	m_arrowHead.setFillColor(sf::Color::Red);

	m_line[0].position = m_startPosition;
	m_line[1].position = m_endPosition;
	m_line[0].color = color;
	m_line[1].color = color;

	m_arrowHead.setPosition(arrowHeadPosition());
	m_arrowHead.rotate(arrowHeadRotation());
}

Arch::Arch(uint16_t id, const std::string& text, const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color) :
	m_line(sf::Lines, 2), m_ID(id), m_startPosition(start), m_endPosition(end), m_color(color), m_text(text)
{
	m_lineSlope = (m_endPosition.y - m_startPosition.y) / (m_endPosition.x - m_startPosition.x);

	m_arrowHead.setRadius(s_arrowHeadRadius);
	m_arrowHead.setPointCount(s_arrowHeadPointCount);
	m_arrowHead.setOrigin(m_arrowHead.getRadius(), m_arrowHead.getRadius());
	m_arrowHead.setFillColor(sf::Color::Red);

	m_line[0].position = m_startPosition;
	m_line[1].position = m_endPosition;
	m_line[0].color = color;
	m_line[1].color = color;

	m_arrowHead.setPosition(arrowHeadPosition());
	m_arrowHead.rotate(arrowHeadRotation());
}

Arch::Arch(uint16_t id, const sf::Vector2f& position, sf::Color color) :
	m_ID(id), m_samePosition(position), m_color(color)
{
	m_circleLinker.setFillColor(sf::Color::Transparent);
	m_circleLinker.setOutlineColor(color);
	m_circleLinker.setOutlineThickness(s_outlineThickness);
	m_circleLinker.setOrigin(m_circleLinker.getRadius() + 25, m_circleLinker.getRadius() + 25);
	m_circleLinker.setPosition(m_samePosition);
	m_circleLinker.setRadius(g_radius + 5.0f);
}

void Arch::drawArchLine(sf::RenderWindow& window) const
{
	// Line
	if (m_lineSlope != 0.0f)
	{
		window.draw(m_line);
		if (!m_text.empty())
		{
			// Font
			sf::Font font;
			font.loadFromFile("font.ttf");

			// Text
			sf::Text archText(m_text, font);
			archText.setFillColor(sf::Color::White);
			archText.setCharacterSize(15);
			archText.setOutlineThickness(2);
			archText.setOutlineColor(sf::Color(0, 0, 0));
			const sf::FloatRect bounds = archText.getLocalBounds();
			archText.setOrigin(bounds.width / 2, bounds.height / 2);
			const sf::Vector2f middlePosition((m_startPosition.x + m_endPosition.x) / 2.0f, (m_startPosition.y + m_endPosition.y) / 2.0f);
			archText.setPosition(middlePosition);
			window.draw(archText);
		}
	}
	else
	{
		window.draw(m_circleLinker);
	}
}

void Arch::drawArchArrowHead(sf::RenderWindow& window) const
{
	window.draw(m_arrowHead);
}

float Arch::arrowHeadRotation() const
{
	return atan(m_lineSlope) * 180 / s_PI;
}

sf::Vector2f Arch::arrowHeadPosition() const
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
