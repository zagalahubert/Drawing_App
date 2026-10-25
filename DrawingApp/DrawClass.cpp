#include "DrawClass.h"
#include "FileInput.h"
#include <iostream>
#include <filesystem> 

constexpr unsigned int NUM_SAVED_UNDO_CHANGES = 10;

//----------------------------------------------------------------------------------------------------------------------private

void DrawClass::handleImageClick(Tools tool)
{
	// change current tool
	if (m_currTool == tool)
		m_currTool = Tools::NONE;
	else
		m_currTool = tool;
}

void DrawClass::fileMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New"))
		{
			m_renderTexture.clear(sf::Color::White);
			m_prevRenderTextureStates.clear();
		}
		if (ImGui::MenuItem("Open","Ctrl+O"))
			m_openFileWindow = true;
		if (ImGui::MenuItem("Save as...", "Ctrl+S"))
			m_saveFileWindow = true;
		ImGui::Separator();
		if (ImGui::MenuItem("Exit"))
			m_shouldClose = true;
		ImGui::EndMenu();
	}
}

void DrawClass::viewMenu()
{
	if (ImGui::BeginMenu("View"))
	{
		ImGui::MenuItem("Show tool options", NULL, &m_additionalToolOptions);
		ImGui::EndMenu();
	}
}

void DrawClass::helpMenu()
{
	if (ImGui::BeginMenu("Help"))
	{
		if (ImGui::MenuItem("About.."))
			m_aboutWindow = true;
		
		ImGui::EndMenu();
	}
}

void DrawClass::checkMenuOptions() 
{
	// Flag toImGui to open popup
	if (m_aboutWindow) ImGui::OpenPopup("About"); m_aboutWindow = false;
	// Flag to ImGuiFileDialog to open SaveFile window
	if (m_saveFileWindow)
	{
		IGFD::FileDialogConfig config;
		config.path = ".";
		ImGuiFileDialog::Instance()->OpenDialog("SaveFile", "Save File", ".png", config);
		m_saveFileWindow = false;
	}
	// Flag to ImGuiFileDialog to open OpenFile window
	if (m_openFileWindow)
	{
		IGFD::FileDialogConfig config;
		config.path = ".";
		ImGuiFileDialog::Instance()->OpenDialog("OpenFile", "Open File", ".png", config);
		m_openFileWindow = false;
	}
}

void DrawClass::showAboutWindow()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2{0.5f, 0.5f});

	if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_NoResize))
	{	
		ImGui::Text("IUiBG Lab02");
		ImGui::Text("Autor: Hubert Zagala");
		if (ImGui::Button("Ok"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
}

void DrawClass::fileGUI()
{
	if (ImGuiFileDialog::Instance()->Display("OpenFile")) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			m_fileDialogOpened = true;
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			sf::Texture texture;
			if (!texture.loadFromFile(filePathName))
			{
				std::cout << "Could not load file: " << filePathName << std::endl;
				return;
			}
			m_prevRenderTextureStates.clear();
			sf::Sprite sprite{ texture };
			m_renderTexture.draw(sprite);
			m_renderTexture.display();
		}

		ImGuiFileDialog::Instance()->Close();
		m_fileDialogOpened = false;
	}

	if (ImGuiFileDialog::Instance()->Display("SaveFile")) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			m_fileDialogOpened = true;
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			if (m_renderTexture.getTexture().copyToImage().saveToFile(filePathName))
				std::cout << "Saved file: " << filePathName << std::endl;
		}

		ImGuiFileDialog::Instance()->Close();
		m_fileDialogOpened = false;
	}
}

// Pushes back current state. Deletes oldest change if number of stored changes > NUM_SAVED_UNDO_CHANGES
void DrawClass::pushRenderTexture()
{
	m_prevRenderTextureStates.push_back(m_renderTexture.getTexture());
	if (m_prevRenderTextureStates.size() > NUM_SAVED_UNDO_CHANGES)
		m_prevRenderTextureStates.pop_front();
}

// Restores latest stored change
void DrawClass::undoRenderTexture()
{	
	if (m_prevRenderTextureStates.empty()) return;

	sf::Texture texture = m_prevRenderTextureStates.back();
	sf::Sprite sprite { texture };
	m_prevRenderTextureStates.pop_back();
	m_renderTexture.clear(sf::Color::White);
	m_renderTexture.draw(sprite);
}

//----------------------------------------------------------------------------------------------------------------------public
DrawClass::DrawClass(std::string& iconsPath, sf::Vector2u windowSize)
	: m_fillColor(0.0f, 0.0f, 0.0f), m_borderColor(0.0f, 0.0f, 0.0f),
	m_windowSize(windowSize), m_renderTexture(windowSize),
	m_iconsDirPath(iconsPath), m_currTool(Tools::NONE),
	m_additionalToolOptions(false), m_aboutWindow(false),
	m_openFileWindow(false), m_saveFileWindow(false),
	m_shouldClose(false), m_mouseLeftPressed(false),
	m_fileDialogOpened(false), m_startCursorPos({0,0}),
	m_currCursorPos({0,0})
{
	// Calculates ImageButton size
	float buttonSize = std::min(windowSize.x / 15.0f, windowSize.y / 15.0f);

	// Clears canvas (default is black)
	m_renderTexture.clear(sf::Color::White);

	// Holds all file paths to icons
	std::vector<std::string> iconFilePaths;
	FileInput::loadFilePathString(m_iconsDirPath, iconFilePaths);

	for (std::string str : iconFilePaths) 
	{
		sf::Texture	tex; 
		if (!tex.loadFromFile(str))
			std::cout << "Icon file: '" + str + "' invalid." << std::endl;

		std::string toolName = FileInput::getNameFromFilePath(str, m_iconsDirPath);
		Tools tool = ToolInterface::getToolFromString(toolName);
		// Adds a tool based on its name
		m_toolsMap.emplace(tool, ToolInterface::genTool(tool, tex, sf::Vector2f{ buttonSize, buttonSize }, m_borderColor, m_fillColor));
	}
}

void DrawClass::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draws canvas
	sf::Sprite sprite{ m_renderTexture.getTexture() };
	target.draw(sprite);
	
	// Acts as a draw animation for all tools, no need for setup as UpdateShape already takes care of it
	if (m_mouseLeftPressed && (m_currTool != Tools::NONE))
	{
		target.draw(*m_toolsMap.at(m_currTool));
		
	}
}

void DrawClass::showMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		fileMenu(); 
		viewMenu(); 
		helpMenu();

		ImGui::EndMainMenuBar();
	}
}

void DrawClass::showTools()
{
	// Tools window
	ImGui::Begin("Tools");

	for (const auto& [tool, ptr] : m_toolsMap)
	{
		ptr->setSelected(m_currTool == tool);

		if (ptr->drawImageButton())
			handleImageClick(tool);

		ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();
	}

	ImGui::Separator();

	ImGui::ColorEdit3("Border color", &m_borderColor.x);
	ImGui::ColorEdit3("Fill color", &m_fillColor.x);

	// Draws when additional tools turned on
	if (m_additionalToolOptions)
	{
		ImGui::Separator();
		if (m_currTool != Tools::NONE)
			m_toolsMap[m_currTool]->drawAdditionalOptions();
	}

	// Draws undo button when previous states saved
	if (!m_prevRenderTextureStates.empty())
	{
		ImGui::Separator();
		if (ImGui::Button("Undo"))
			undoRenderTexture();
	}

	ImGui::End();
}

// Calls functions that draw when called by ImGui and ImGuiFileDialog
void DrawClass::showModals()
{
	showAboutWindow();
	fileGUI();
}

// Calls functions that update current shape that is drawn, handle menu as well as tools window and their actions.
void DrawClass::onUpdate()
{
	updateShape();
	showMenu();
	checkMenuOptions();
	showTools();
	showModals();
}

// Draws on canvas when mouse left click is realesed
void DrawClass::drawOnCanvas()
{
	m_renderTexture.draw(*m_toolsMap[m_currTool]);
	m_renderTexture.display();
	return;
}

// Handles all the logic behind drawing animation
void DrawClass::updateShape()
{
	// If mouse left click is not pressed then return
	if (!m_mouseLeftPressed) return;
	
	if (m_currTool != Tools::NONE)
	{
		m_toolsMap[m_currTool]->updateInputs(m_startCursorPos, m_currCursorPos, m_borderColor, m_fillColor);
		m_toolsMap[m_currTool]->updateShape();
	}
	
	return;
}

// Function handles mouse inputs and calls drawing on canvas when mouse left clicked released
void DrawClass::handleEvent(std::optional<sf::Event> event)
{
	if (ImGui::GetIO().WantCaptureMouse || m_fileDialogOpened)
		return;

	if (const auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>())
	{
		switch (mouseEvent->button)
		{
		case (sf::Mouse::Button::Left):
			// Keeps starting point of mouse left click press if tool selected
			if (m_currTool != Tools::NONE)
			{
				m_startCursorPos = mouseEvent->position;
				m_currCursorPos = mouseEvent->position;
			}
			// Flags to other functions that mouse left click is pressed
			m_mouseLeftPressed = true;
			break;
		}
	}

	// Updates cursor position every time it moves
	if (const auto mouseEvent = event->getIf<sf::Event::MouseMoved>())
		m_currCursorPos = mouseEvent->position;

	if (const auto mouseEvent = event->getIf<sf::Event::MouseButtonReleased>())
	{
		switch (mouseEvent->button)
		{
		case (sf::Mouse::Button::Left):
			// If current tool picked saves previous state and draws on canvas
			if (m_currTool != Tools::NONE)
			{ 
				pushRenderTexture();
				drawOnCanvas();
			}
			m_mouseLeftPressed = false;
			break;
		}
	}

	if (const auto keyEvent = event->getIf<sf::Event::KeyPressed>())
	{
		if (keyEvent->control)
		{
			switch (keyEvent->code)	
			{
			case sf::Keyboard::Key::S:
				m_saveFileWindow = true;
				break;
			case sf::Keyboard::Key::O:
				m_openFileWindow = true;
				break;
			case sf::Keyboard::Key::Z:
				undoRenderTexture();
				break;
			}
		}
	}
}