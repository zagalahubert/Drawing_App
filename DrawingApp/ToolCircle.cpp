#include "ToolCircle.h"

ToolCircle::ToolCircle(Tools tool, sf::Texture texture, sf::Vector2f imageButtonSize,
	sf::Vector3f& borderColor, sf::Vector3f& fillColor)
	: ToolInterface(tool, "Draws empty circle", texture, imageButtonSize, 
		borderColor, fillColor),
	m_thickness(1.0f)
{
}

void ToolCircle::drawAdditionalOptions()
{
	ImGui::SliderFloat("Border thickness", &m_thickness, 1.0f, 10.0f);
}

void ToolCircle::drawOnCanvas(sf::RenderTexture& m_renderTexture)
{
	m_renderTexture.draw(m_circle);
}

void ToolCircle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_circle);
}

void ToolCircle::updateShape()
{
	sf::Vector2f start = { static_cast<float>(m_startCursorPos.x), static_cast<float>(m_startCursorPos.y) };
	sf::Vector2f end = { static_cast<float>(m_currCursorPos.x), static_cast<float>(m_currCursorPos.y) };

	sf::Color borderColor = sf::Color(static_cast<std::uint8_t>(m_borderColor.x * 255),
		static_cast<std::uint8_t>(m_borderColor.y * 255), static_cast<std::uint8_t>(m_borderColor.z * 255));

	float width = std::abs(start.x - end.x);
	float height = std::abs(start.y - end.y);
	float squareSize = std::min(width, height);
	sf::Vector2f circPos{ start.x - (end.x < start.x ? squareSize : 0.0f),
						  start.y - (end.y < start.y ? squareSize : 0.0f) };

	m_circle.setFillColor(sf::Color::Transparent);
	m_circle.setOutlineThickness(m_thickness);
	m_circle.setOutlineColor(borderColor);
	m_circle.setPosition(circPos);
	m_circle.setRadius(std::min(width, height) / 2.0f);
}
