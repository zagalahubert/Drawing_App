#include "DrawClass.h"
#include <iostream>

int main() {
    sf::Vector2u windowSize({ 1800, 900 });
    sf::RenderWindow window(sf::VideoMode(windowSize), "DrawingApp");
    window.setMinimumSize(windowSize);
    window.setMinimumSize(windowSize);
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
    {
        std::cout << "Failed to initialize SFML ImGui." << std::endl;
        return 1;
    }
        
    std::string iconsPath = "res/icons/";
    DrawClass drawClass(iconsPath, windowSize);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        if (drawClass.shouldWindowClose()) window.close();

        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            drawClass.handleEvent(event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart() );

        drawClass.onUpdate();

        window.clear();
        window.draw(drawClass);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
