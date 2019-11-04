#include <iostream>
#include <SFML/Graphics.hpp>

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Mandelbrot.h"
#include "DrawingManager.h"
#include "Logger.h"


using std::vector;

/// <summary>
/// Run the interactive version of the mandelbrot without benchmarking.
/// </summary>
void RunInteractiveMandelbrot()
{
	Logger::IsBenchmarking = false;

	sf::RenderWindow window(sf::VideoMode(1024, 768 + DrawingManager::GUIHeight), "CMP202 Interactive Mandelbrot");
	window.setFramerateLimit(30);
	Mandelbrot::window = &window;

	Logger::AddHeaders();

	DrawingManager drawingManager(&window, 16, 200);
	drawingManager.Redraw();

	while (window.isOpen())
	{
		window.clear();
		drawingManager.Run();
		drawingManager.ProcessInput();
		drawingManager.DrawTiles();
		drawingManager.DrawGUI();
		window.display();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
	}
}

/// <summary>
/// Run a single benchmark on the mandelbrot. No Logger here.
/// </summary>
/// <param name="segments">How many segments the image should have.</param>
/// <param name="startIterations">How many iterations there should be.</param>
/// <param name="testRepeats">How many times the test should be repeated.</param>
void RunMandelbrotBench(int segments, int startIterations, int testRepeats)
{
	sf::RenderWindow window(sf::VideoMode(1024, 768 + DrawingManager::GUIHeight), "CMP202 Interactive Mandelbrot");
	window.setFramerateLimit(30);
	Mandelbrot::window = &window;

	DrawingManager drawingManager(&window, segments, startIterations);
	drawingManager.Redraw();

	int doneCount = 0;

	while (window.isOpen())
	{
		window.clear();
		drawingManager.Run();
		if (!Logger::IsBenchmarking)
		{
			drawingManager.ProcessInput();
		}
		drawingManager.DrawTiles();
		drawingManager.DrawGUI();
		window.display();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		if (drawingManager.IsDone())
		{
			doneCount++;

			Logger::CurrentRun = doneCount;

			drawingManager.Redraw();
		}

		if (doneCount > testRepeats)
			window.close();
	}
}

/// <summary>
/// Run all the data as seen on the Performance Measurement slides.
/// </summary>
void RunStatistics()
{
	Logger::IsBenchmarking = true;
	Logger::AddHeaders();

	/*Time taken vs Tiles with iterations*/

	for (int iterations = 200; iterations <= 600; iterations += 100)
	{
		for (int i = 1; i < 24; i++)
		{
			RunMandelbrotBench(i, iterations, 5);
		}

		std::stringstream str;
		str << " Iterations vs Time " << iterations << " Iterations";
		Logger::SaveAll(str.str());
		Logger::ResetContentsAndAddHeaders();
	}

	/*Time taken vs Tiles with iterations ZOOMED */

	Mandelbrot::TotalRect.Translation = { -0.748, 0.1 };
	Mandelbrot::TotalRect.Scale = (0.0014);

	for (int iterations = 200; iterations <= 600; iterations += 100)
	{
		for (int i = 1; i < 24; i++)
		{
			Mandelbrot::TotalRect.Translation = { -0.748, 0.1 };
			Mandelbrot::TotalRect.Scale = (0.0014);
			RunMandelbrotBench(i, iterations, 5);
		}

		std::stringstream str;
		str << " Iterations vs Time Zoomed " << iterations << " Iterations";
		Logger::SaveAll(str.str());
		Logger::ResetContentsAndAddHeaders();
	}
}

/// <summary>
/// A reduced benchmark which just focuses on redrawing the Mandelbrot set a particular amount of times.
/// </summary>
void RunProfiler()
{
	RunMandelbrotBench(8, 500, 50);
}

int main()
{
	//Mandelbrot::TotalRect.Translation = { -0.748, 0.1 };
	//Mandelbrot::TotalRect.Scale = (0.0014);

	RunInteractiveMandelbrot(); //UNCOMMENT ME!

	//RunStatistics();

	//RunProfiler();

	return 0;
}