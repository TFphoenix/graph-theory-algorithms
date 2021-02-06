#include "UIButton.h"

UIButton::UIButton(const std::string& text, const sf::Vector2f& position, const sf::Vector2f& size) :
	m_isMouseIn(false),
	m_state(State::None),
	m_body(size),
	m_collider(position, size),
	m_text(text)
{
	// RectangleShape
	m_body.setPosition(position);
	m_body.setFillColor(sf::Color(0, 0, 0, 0));
	m_body.setOutlineColor(sf::Color(200, 200, 20));
	m_body.setOutlineThickness(2);

	// Font
	m_font.loadFromFile("font.ttf");

	// Text
	m_UIText.setString(text);
	m_UIText.setFont(m_font);
	m_UIText.setCharacterSize(20);
	const auto bounds = m_UIText.getGlobalBounds();
	m_UIText.setOrigin(bounds.width / 2, bounds.height / 2);
	m_UIText.setPosition(m_body.getPosition().x + m_body.getSize().x / 2, m_body.getPosition().y + m_body.getSize().y / 2 - 5);
	m_UIText.setFillColor(sf::Color(200, 200, 20));

}

void UIButton::ChangeState(State newState)
{
	m_state = newState;
	if (newState == State::Disabled)
	{
		m_body.setFillColor(sf::Color(0, 0, 0, 0));
		m_body.setOutlineColor(sf::Color(100, 100, 100));
		m_UIText.setFillColor(sf::Color(100, 100, 100));
	}
	else if (newState == State::None)
	{
		m_body.setFillColor(sf::Color(0, 0, 0, 0));
		m_body.setOutlineColor(sf::Color(200, 200, 20));
		m_UIText.setFillColor(sf::Color(200, 200, 20));
	}
}

UIButton::State UIButton::GetState() const
{
	return m_state;
}

void UIButton::ChangeText(const std::string& newText)
{
	m_UIText.setString(newText);
	const auto bounds = m_UIText.getGlobalBounds();
	m_UIText.setOrigin(bounds.width / 2, bounds.height / 2);
	m_UIText.setPosition(m_body.getPosition().x + m_body.getSize().x / 2, m_body.getPosition().y + m_body.getSize().y / 2 - 5);
}

void UIButton::HandleEvent(const sf::Event& e)
{
	const bool wasMouseIn = m_isMouseIn;
	switch (e.type)
	{
	case sf::Event::MouseMoved:
		m_isMouseIn = m_collider.contains(e.mouseMove.x, e.mouseMove.y);
		break;
	case sf::Event::MouseButtonPressed:
		if (m_isMouseIn)
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				OnMouseLeftClick();
			}
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (m_isMouseIn)
		{
			if (e.mouseButton.button == sf::Mouse::Left)
			{
				OnMouseLeftRelease();
			}
		}
		break;
	default:
		break;
	}

	if (wasMouseIn)
	{
		if (!m_isMouseIn)
		{
			OnMouseLeave();
		}
	}
	else
	{
		if (m_isMouseIn)
		{
			OnMouseEnter();
		}
	}
}

void UIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_body);
	target.draw(m_UIText);
}

void UIButton::OnMouseEnter()
{
	if (m_state == State::Disabled)
		return;
	m_state = State::Hover;
	m_body.setFillColor(sf::Color(200, 200, 20));
	m_body.setOutlineColor(sf::Color(255, 255, 255));
	m_UIText.setFillColor(sf::Color(255, 255, 255));
}

void UIButton::OnMouseLeave()
{
	if (m_state == State::Disabled)
		return;
	m_state = State::None;
	m_body.setFillColor(sf::Color(0, 0, 0, 0));
	m_body.setOutlineColor(sf::Color(200, 200, 20));
	m_UIText.setFillColor(sf::Color(200, 200, 20));
}

void UIButton::OnMouseLeftClick()
{
	if (m_state == State::Disabled)
		return;
	m_state = State::Click;
	m_body.setFillColor(sf::Color(150, 150, 0));
	m_body.setOutlineColor(sf::Color(200, 200, 200));
	m_UIText.setFillColor(sf::Color(200, 200, 200));
}

void UIButton::OnMouseLeftRelease()
{
	if (m_state == State::Disabled)
		return;
	m_state = State::Release;
	m_body.setFillColor(sf::Color(200, 200, 20));
	m_body.setOutlineColor(sf::Color(255, 255, 255));
	m_UIText.setFillColor(sf::Color(255, 255, 255));
}
