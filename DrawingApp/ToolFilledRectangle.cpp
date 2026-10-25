#include "ToolFilledRectangle.h"

ToolFilledRectangle::ToolFilledRectangle(Tools tool, sf::Texture texture, sf::Vector2f imageButtonSize,
	sf::Vector3f& borderColor, sf::Vector3f& fillColor)
	: ToolInterface(tool, "Draws filled rectangle", texture, imageButtonSize, 
		borderColor, fillColor),
	m_thickness(1.0f)
{
}

void ToolFilledRectangle::drawAdditionalOptions()
{
	ImGui::SliderFloat("Border thickness", &m_thickness, 1.0f, 10.0f);
}

void ToolFilledRectangle::drawOnCanvas(sf::RenderTexture& m_renderTexture)
{
	m_renderTexture.draw(m_filledRectangle);
}

void ToolFilledRectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_filledRectangle);
}

void ToolFilledRectangle::updateShape()
{
	sf::Vector2f start = { static_cast<float>(m_startCursorPos.x), static_cast<float>(m_startCursorPos.y) };
	sf::Vector2f end = { static_cast<float>(m_currCursorPos.x), static_cast<float>(m_currCursorPos.y) };

	sf::Color borderColor = sf::Color(static_cast<std::uint8_t>(m_borderColor.x * 255),
		static_cast<std::uint8_t>(m_borderColor.y * 255), static_cast<std::uint8_t>(m_borderColor.z * 255));
	sf::Color fillColor = sf::Color(static_cast<std::uint8_t>(m_fillColor.x * 255),
		static_cast<std::uint8_t>(m_fillColor.y * 255), static_cast<std::uint8_t>(m_fillColor.z * 255));

	sf::Vector2f rectPos{ std::min(start.x, end.x), std::min(start.y, end.y) };
	sf::Vector2f rectSize{ std::abs(start.x - end.x), std::abs(start.y - end.y) };

	m_filledRectangle.setFillColor(fillColor);
	m_filledRectangle.setOutlineColor(borderColor);
	m_filledRectangle.setOutlineThickness(m_thickness);
	m_filledRectangle.setPosition(rectPos);
	m_filledRectangle.setSize(rectSize);
}
