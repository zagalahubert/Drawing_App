#pragma once
#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

enum class Tools {
	NONE = -1,
	BROOM = 0,
	CIRCLE = 1,
	SQUARE = 2,
	WINDOW = 3
};

class ToolInterface : public sf::Drawable
{
public:
	ToolInterface(Tools tool, std::string description, sf::Texture texture, 
		sf::Vector2f imageButtonSize, sf::Vector3f& borderColor, sf::Vector3f& fillColor);
	virtual ~ToolInterface() = default;

	static Tools getToolFromString(const std::string& s);
	static std::string getStringFromTool(Tools tool);
	static std::unique_ptr<ToolInterface> genTool(Tools tool, sf::Texture, 
		sf::Vector2f imageButtonSize, sf::Vector3f& borderColor, sf::Vector3f& fillColor);

	bool drawImageButton();
	void setSelected(bool s);
	void updateInputs(sf::Vector2i startCursorPos, sf::Vector2i currCursorPos,
		sf::Vector3f borderColor, sf::Vector3f fillColor);
	inline const Tools getTool() const { return m_tool; };
	inline const std::string getDescription() const { return m_description; };

	virtual void updateShape() = 0;
	virtual void drawAdditionalOptions() = 0;
	virtual void drawOnCanvas(sf::RenderTexture& m_renderTexture) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
protected:
	Tools m_tool;
	std::string m_description;
	sf::Texture m_texture;
	bool m_selected;
	sf::Vector2f m_imageButtonSize;
	sf::Vector2i m_startCursorPos, m_currCursorPos;
	sf::Vector3f m_borderColor, m_fillColor;
};

