#pragma once
#include "ToolInterface.h"
class ToolCircle : public ToolInterface
{
public:
	ToolCircle(Tools tool, sf::Texture texture,
		sf::Vector2f imageButtonSize, sf::Vector3f& borderColor, sf::Vector3f& fillColor);
	void drawAdditionalOptions() override;
	void drawOnCanvas(sf::RenderTexture& m_renderTexture) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateShape() override;
private:
	sf::CircleShape m_circle;
	float m_thickness;
};

