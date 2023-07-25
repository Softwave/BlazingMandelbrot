/*
    Blazing Mandelbrot 

    Cool little program that displays a fiery mandelbrot

    MIT License

    Copyright (c) 2021 Jessica Leyba

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>
#include <string>

const int W = 800;
const int H = 600;

// So we can save screenshots without the GUI 
bool canDrawGUI = true; 
bool canDrawAbout = false;

bool saveScreenshot = false; 

// Shader uniforms 
float timeUniform = 0.0f; 
int iterationUniform = 64; 
float zoomUniform = 0.1f;
float zoomPosXUniform = 0.755f;
float panModUniform = 0.0f;

int main()
{
    sf::RenderWindow window(sf::VideoMode(W, H), "Blazing Mandelbrot");
	ImGui::SFML::Init(window); 
	
	// Setup the icon 
	sf::Image icon;
	icon.loadFromFile("mainicon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	sf::Texture tex;
	tex.create(W, H);
	sf::Sprite spr(tex);

	// Load the shader 
	sf::Shader shader;
	shader.loadFromFile("frag.glsl", sf::Shader::Fragment);
	if (!shader.isAvailable())
	{
		std::cout << "The shader is not avaliable.\n";
	}

	// Clocks, one for the user interface and one for the shader 
	sf::Clock guiClock;
	sf::Clock shaderClock; 
	shaderClock.restart();

	// Set the resolution uniform
	shader.setUniform("resolution", sf::Vector2f(W, H));

	// Style (made with the ImGui Style Editor 
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f; 
	style.WindowBorderSize = 0.0f; 
	style.ScrollbarRounding = 0.0f; 
	ImVec4* colors = style.Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(0.29f, 0.31f, 0.87f, 0.94f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.06f, 0.73f, 0.10f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.06f, 0.73f, 0.10f, 0.40f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.52f, 0.28f, 0.81f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.52f, 0.28f, 0.81f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.85f, 0.17f, 0.29f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.17f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.85f, 0.17f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.85f, 0.17f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.85f, 0.17f, 0.31f, 1.00f);

    // Add this flag to handle saving the screenshot
    bool saveScreenshot = false;

    // Main loop
    while (window.isOpen())
    {
        // Poll for events
        sf::Event e;
        while (window.pollEvent(e))
        {
            ImGui::SFML::ProcessEvent(e);
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }

            // Handle other events here
            // ...
            // Reset the resolution uniform when we maximize 
			if (e.type == sf::Event::Resized)
			{
				shader.setUniform("resolution", sf::Vector2f(e.size.width, e.size.height));
			}

			// 
			if (e.type == sf::Event::KeyPressed)
			{
				// Save a screenshot if we press 's'
				if (e.key.code == sf::Keyboard::S)
				{
                    saveScreenshot = true; 
					canDrawGUI = false; 
				}
			}
        }

        // Start a new ImGui frame
        ImGui::SFML::Update(window, guiClock.restart());


        // Draw the GUI
        if (canDrawGUI)
        {
            ImGui::Begin("Blazing Mandelbrot Controls");
            ImGui::SliderInt("Iterations", &iterationUniform, 0, 300);
            ImGui::SliderFloat("Zoom", &zoomUniform, 0.0f, 50.0f);
            ImGui::SliderFloat("Pos X", &zoomPosXUniform, 0.6f, 0.8f);
            ImGui::SliderFloat("Pan Mod", &panModUniform, 0.0f, 0.35f);
            if (ImGui::Button("Reset"))
            {
                timeUniform = 0.0f;
                iterationUniform = 64;
                zoomUniform = 0.1f;
                zoomPosXUniform = 0.755f;
                panModUniform = 0.0f;
            }
            if (ImGui::Button("Save Screenshot"))
            {
                saveScreenshot = true;
                canDrawGUI = false;
            }
            if (ImGui::Button("About"))
            {
                canDrawAbout = true;
            }
            ImGui::End();
        }

        // If the "About" window is open, draw it
        if (canDrawAbout)
        {
            ImGui::Begin("About");
            ImGui::Text("Blazing Mandelbrot v 1.2");
            ImGui::Separator();
            ImGui::Text("By Jessica Leyba 2023");
            ImGui::Text("s0ftwave.com");
            ImGui::Text("Check the readme and license files for more info");
            if (ImGui::Button("Close"))
            {
                canDrawAbout = false;
            }
            ImGui::End();
        }

        // Set the time value for the time uniform
        timeUniform = shaderClock.getElapsedTime().asSeconds() * 2.0;

        // Set the uniforms
        shader.setUniform("time", timeUniform);
        shader.setUniform("iterations", iterationUniform);
        shader.setUniform("zoom", zoomUniform);
        shader.setUniform("zoomPosX", zoomPosXUniform);
        shader.setUniform("panMod", panModUniform);

        // Render the scene to the window
        window.clear();
        window.draw(spr, &shader);

        // ImGui rendering pass (after the main rendering pass)
        if (canDrawGUI)
        {
            ImGui::SFML::Render(window);
        }
        

        // Save a screenshot if the flag is set
        if (!canDrawGUI && saveScreenshot)
        {
            ImGui::Render();
            ImGui::EndFrame();
            
            sf::Texture screenTex;
            screenTex.create(window.getSize().x, window.getSize().y);
            screenTex.update(window);
            
            // The screenshot's filename will be the current date and time
            time_t rawtime;
            struct tm* timeinfo;
            char filename[80];
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(filename, 80, "screen-%Y-%m-%d-%H-%M-%S.png", timeinfo);

            if (screenTex.copyToImage().saveToFile(filename))
            {
                std::cout << "Screenshot saved as " << filename << std::endl;
            }

            saveScreenshot = false;
            canDrawGUI = true; // Allow GUI rendering again
        }

        window.display();

    }

    // Shutdown ImGui correctly
    ImGui::SFML::Shutdown();

    return 0;
}
