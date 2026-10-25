#include "ToolLine.h"

ToolLine::ToolLine(Tools tool, sf::Texture texture, sf::Vector2f imageButtonSize, 
	sf::Vector3f& borderColor, sf::Vector3f& fillColor)
	: ToolInterface(tool, "Draws straight line", texture, imageButtonSize, borderColor, 
		fillColor),
	m_useFillColor(false), m_line(sf::PrimitiveType::Lines, 2)
{
}

void ToolLine::drawAdditionalOptions()
{
	ImGui::Checkbox("Use border and fill color", &m_useFillColor);
}

void ToolLine::drawOnCanvas(sf::RenderTexture& m_renderTexture)
{
	m_renderTexture.draw(m_line);
}

void ToolLine::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_line);
}

void ToolLine::updateShape()
{
	sf::Vector2f start = { static_cast<float>(m_startCursorPos.x), static_cast<float>(m_startCursorPos.y) };
	sf::Vector2f end = { static_cast<float>(m_currCursorPos.x), static_cast<float>(m_currCursorPos.y) };

	sf::Color borderColor = sf::Color(static_cast<std::uint8_t>(m_borderColor.x * 255),
		static_cast<std::uint8_t>(m_borderColor.y * 255), static_cast<std::uint8_t>(m_borderColor.z * 255));
	sf::Color fillColor = sf::Color(static_cast<std::uint8_t>(m_fillColor.x * 255),
		static_cast<std::uint8_t>(m_fillColor.y * 255), static_cast<std::uint8_t>(m_fillColor.z * 255));

	m_line[0].position = start;
	m_line[0].color = borderColor;
	m_line[1].position = end;
	// Use fill color if turned on
	m_line[1].color = (m_useFillColor ? fillColor : borderColor);
}
