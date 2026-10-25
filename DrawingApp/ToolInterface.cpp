#include "ToolInterface.h"

#include "ToolCircle.h"
#include "ToolLine.h"
#include "ToolRectangle.h"
#include "ToolFilledRectangle.h"

ToolInterface::ToolInterface(Tools tool, std::string description, sf::Texture texture,
	sf::Vector2f imageButtonSize, sf::Vector3f& borderColor, sf::Vector3f& fillColor)
	: m_selected(false), m_tool(tool), m_description(description), m_texture(texture),
	m_imageButtonSize(imageButtonSize), m_fillColor(fillColor), m_borderColor(borderColor),
	m_startCursorPos({0,0}), m_currCursorPos({0,0})
{
}

Tools ToolInterface::getToolFromString(const std::string& s)
{
	if (s == "broom") return Tools::BROOM;
	if (s == "circle") return Tools::CIRCLE;
	if (s == "square") return Tools::SQUARE;
	if (s == "window") return Tools::WINDOW;

	return Tools::NONE;
}

std::string ToolInterface::getStringFromTool(Tools tool)
{
	switch (tool) {
	case Tools::NONE:
		return "none";
	case Tools::BROOM:
		return "broom";
	case Tools::CIRCLE:
		return "circle";
	case Tools::SQUARE:
		return "square";
	case Tools::WINDOW:
		return "window";
	}
	return "Unknown";
}

std::unique_ptr<ToolInterface> ToolInterface::genTool(Tools tool, sf::Texture texture, sf::Vector2f imageButtonSize,
	sf::Vector3f& borderColor, sf::Vector3f& fillColor)
{
	switch (tool)
	{
		case Tools::BROOM:
			return std::make_unique<ToolLine>(tool, texture, imageButtonSize, borderColor, fillColor);
			break;
		case Tools::CIRCLE:
			return std::make_unique<ToolCircle>(tool, texture, imageButtonSize, borderColor, fillColor);
			break;
		case Tools::SQUARE:
			return std::make_unique<ToolRectangle>(tool, texture, imageButtonSize, borderColor, fillColor);
			break;
		case Tools::WINDOW:
			return std::make_unique<ToolFilledRectangle>(tool, texture, imageButtonSize, borderColor, fillColor);
			break;
	}
	return nullptr;
}

bool ToolInterface::drawImageButton()
{
	bool state = ImGui::ImageButton(getStringFromTool(m_tool).c_str(), m_texture, m_imageButtonSize,
		(m_selected ? sf::Color::Cyan : sf::Color::White));
	ImGui::SetItemTooltip(m_description.c_str());
	return state;
}

void ToolInterface::setSelected(bool s)
{
	m_selected = s;
}

void ToolInterface::updateInputs(sf::Vector2i startCursorPos, sf::Vector2i currCursorPos, 
	sf::Vector3f borderColor, sf::Vector3f fillColor)
{
	m_startCursorPos = startCursorPos;
	m_currCursorPos = currCursorPos;
	m_borderColor = borderColor;
	m_fillColor = fillColor;
}
