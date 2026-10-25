#include "ToolRectangle.h"

ToolRectangle::ToolRectangle(Tools tool, sf::Texture texture, sf::Vector2f imageButtonSize,
	sf::Vector3f& borderColor, sf::Vector3f& fillColor)
	: ToolInterface(tool, "Draws empty rectangle", texture, imageButtonSize, 
		borderColor, fillColor),
	m_thickness(1.0f)
{
}

void ToolRectangle::drawAdditionalOptions()
{
	ImGui::SliderFloat("Border thickness", &m_thickness, 1.0f, 10.0f);
}

void ToolRectangle::drawOnCanvas(sf::RenderTexture& m_renderTexture)
{
	m_renderTexture.draw(m_rectangle);
}

void ToolRectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_rectangle);
}

void ToolRectangle::updateShape()
{
	sf::Vector2f start = { static_cast<float>(m_startCursorPos.x), static_cast<float>(m_startCursorPos.y) };
	sf::Vector2f end = { static_cast<float>(m_currCursorPos.x), static_cast<float>(m_currCursorPos.y) };

	sf::Color borderColor = sf::Color(static_cast<std::uint8_t>(m_borderColor.x * 255),
		static_cast<std::uint8_t>(m_borderColor.y * 255), static_cast<std::uint8_t>(m_borderColor.z * 255));

	sf::Vector2f rectPos{ std::min(start.x, end.x), std::min(start.y, end.y) };
	sf::Vector2f rectSize{ std::abs(start.x - end.x), std::abs(start.y - end.y) };

	m_rectangle.setFillColor(sf::Color::Transparent);
	m_rectangle.setOutlineColor(borderColor);
	m_rectangle.setOutlineThickness(m_thickness);
	m_rectangle.setPosition(rectPos);
	m_rectangle.setSize(rectSize);
}
