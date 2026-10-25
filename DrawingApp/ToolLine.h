#pragma once
#include "ToolInterface.h"

class ToolLine : public ToolInterface
{
public:
	ToolLine(Tools tool, sf::Texture texture,
		sf::Vector2f imageButtonSize, sf::Vector3f& borderColor, sf::Vector3f& fillColor);
	void drawAdditionalOptions() override;
	void drawOnCanvas(sf::RenderTexture& m_renderTexture) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateShape() override;
private:
	sf::VertexArray m_line;
	bool m_useFillColor;
};

