#pragma once
#include "ToolInterface.h"

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "imgui/ImGuiFileDialog.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>
#include <string>
#include <memory>
#include <deque>


class DrawClass : public sf::Drawable
{
private:
	
	void handleImageClick(Tools tool); //do nadpisania
	void drawImageButton(Tools tool); //do nadpisania
	void fileMenu();
	void viewMenu();
	void helpMenu();
	void checkMenuOptions();
	void showAboutWindow();
	void fileGUI();
	void pushRenderTexture();
	void undoRenderTexture();
public:
	DrawClass(std::string& iconsPath, sf::Vector2u windowSize);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void showMenu();
	void showTools();
	void showModals();
	void onUpdate();
	void drawOnCanvas();
	void updateShape();
	void handleEvent(std::optional<sf::Event> event);
	inline const bool& shouldWindowClose() const { return m_shouldClose; }
private:
	bool m_additionalToolOptions, m_aboutWindow, m_openFileWindow,
		m_saveFileWindow, m_shouldClose, m_mouseLeftPressed,
		m_fileDialogOpened;
	Tools m_currTool;
	std::string& m_iconsDirPath;
	std::map<Tools,std::unique_ptr<ToolInterface>> m_toolsMap; 
	std::deque<sf::Texture> m_prevRenderTextureStates;
	sf::RenderTexture m_renderTexture;
	sf::Vector3f m_borderColor, m_fillColor;
	sf::Vector2i m_startCursorPos, m_currCursorPos;
	sf::Vector2u m_windowSize; 
};

