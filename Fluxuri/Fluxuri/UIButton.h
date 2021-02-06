#pragma once
#include <SFML/Graphics.hpp>

class UIButton : sf::Drawable
{
public:
	enum class State
	{
		None,
		Hover,
		Click,
		Release,
		Disabled
	};

public:
	UIButton(const std::string& text, const sf::Vector2f& position = sf::Vector2f(10, 10), const sf::Vector2f& size = sf::Vector2f(150, 50));

	void ChangeState(State newState);
	State GetState() const;
	void ChangeText(const std::string& newText);

	void HandleEvent(const sf::Event& e);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates()) const override;

private:
	void OnMouseEnter();
	void OnMouseLeave();
	void OnMouseLeftClick();
	void OnMouseLeftRelease();

private:
	bool m_isMouseIn;
	State m_state;
	sf::RectangleShape m_body;
	sf::Rect<float> m_collider;
	std::string m_text;
	sf::Text m_UIText;
	sf::Font m_font;
};

