// SFML_Setup.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

int main()
{
	using namespace sf;

	ContextSettings settings;
	settings.antialiasingLevel = 8;
	settings.attributeFlags = 0;
	settings.depthBits = 24;
	settings.majorVersion = 4;
	settings.minorVersion = 2;
	settings.stencilBits = 8;

	Window window(VideoMode(800, 600), "OpenGL", Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	float red = 1.0;
	float green = 0;
	float blue = 0;

	bool running = true;
	while (running)
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				running = false;
			}
			else if (event.type == Event::Resized)
			{
				glViewport(0, 0, event.size.width, event.size.height);
			}
			else if (event.type == Event::KeyPressed)
			{
				Keyboard::Key key = event.key.code;
				if (key == Keyboard::Num1)
				{
					red = 1.0;
					green = 0.0;
					blue = 0.0;
				}
				else if (key == Keyboard::Num2)
				{
					red = 0.0;
					green = 1.0;
					blue = 0.0;
				}
				else if (key == Keyboard::Num3)
				{
					red = 0.0;
					green = 0.0;
					blue = 1.0;
				}
			}
		}

		glClearColor(red, green, blue, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		window.display();

		sleep(milliseconds(8));
	}

	return 0;
}

