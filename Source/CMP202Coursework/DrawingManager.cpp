#include "DrawingManager.h"

#include "RedrawTileTask.h"
#include "DesaturateTileTask.h"

#include "Logger.h"

using std::chrono::steady_clock;
using std::chrono::milliseconds;
using std::chrono::duration;
using std::chrono::duration_cast;


DrawingManager::DrawingManager(sf::RenderWindow* window, unsigned int segments, int startIterations) :
	window(window),
	segments(segments),
	taskFarm(0),
	zoomAmount(0.95f),
	moveAmount(0.1f),

	timerRunning(false),
	isDone(false) //14 dec 26 feb
{
	//Set start iterations for mandelbrot and then the iterations as thats what fixes it
	Mandelbrot::START_ITERATIONS = startIterations;
	Mandelbrot::ITERATIONS = Mandelbrot::START_ITERATIONS;

	//Say how many iterations we have to the logger
	Logger::Segments = segments;

	//create tiles for the mandelbrot to be drawn to - looks nicer than lines :)
	int xSegments = static_cast<int>(ceil(window->getSize().x / segments));
	int ySegments = ceil((window->getSize().y - GUIHeight) / segments);

	for (int y = 0; y < segments; y++)
	{
		for (int x = 0; x < segments; x++)
		{
			tiles.push_back(Tile(sf::IntRect(x * xSegments, y * ySegments, xSegments, ySegments), {x + 1, y + 1}, Mandelbrot::ITERATIONS));
		}
	}

	//load font
	font.loadFromFile("Bellefair-Regular.ttf");

	//setup controls text (explains what controls are to the program)
	controlsText.setFont(font);
	controlsText.setCharacterSize(24);
	controlsText.setString(L"+ and - to zoom, arrow keys to move around, r to reset view.");
	controlsText.setPosition(window->getSize().x - controlsText.getLocalBounds().width, window->getSize().y - controlsText.getLocalBounds().height - 5);

	//setup drawing status test (says whether the program has any tasks queued)
	drawingStatusText.setFont(font);
	drawingStatusText.setCharacterSize(24);
	drawingStatusText.setString(L"Tasks Queued");
	drawingStatusText.setPosition(0, window->getSize().y - drawingStatusText.getLocalBounds().height - 5);
}

DrawingManager::~DrawingManager()
{
	Mandelbrot::TotalRect.Reset();
}

void DrawingManager::DrawTiles()
{
	for (auto& tile : tiles)
		window->draw(tile);
}

void DrawingManager::Redraw()
{
	Mandelbrot::TotalRect.UpdateTranslated();

	isDone = false;

	taskFarm.ClearTasks();

	for (auto& tile : tiles) //first colour everything to be red.
		taskFarm.AddTask(new DesaturateTileTask(&tile));

	for (auto& tile : tiles) //then start rendering
		taskFarm.AddTask(new RedrawTileTask(&tile));

	timerRunning = true;
	beginRedrawTimePointCPU = std::clock();
	beginRedrawTimePoint = steady_clock::now();
}

void DrawingManager::Run()
{
	taskFarm.Run();
	std::stringstream drawingStatusStr;
	drawingStatusStr << "Iterations: " << std::to_string(Mandelbrot::ITERATIONS) << "; ";
	if (taskFarm.IsTaskAvaliable() && taskFarm.IsAllThreadsAsleep())
		drawingStatusStr << "Tasks Queued, Threads Sleeping";
	else if (taskFarm.IsTaskAvaliable() && !taskFarm.IsAllThreadsAsleep())
		drawingStatusStr << "Tasks Queued, Thread(s) Working";
	else if (!taskFarm.IsTaskAvaliable() && !taskFarm.IsAllThreadsAsleep())
		drawingStatusStr << "Tasks Empty, Thread(s) Working";
	else if (!taskFarm.IsTaskAvaliable() && taskFarm.IsAllThreadsAsleep()) //if all tasks done
	{
		isDone = true;
		if (timerRunning)
		{
			finishedRedrawTimePointCPU = std::clock();
			finishedRedrawTimePoint = steady_clock::now();
			Logger::AddCPUTime(finishedRedrawTimePointCPU - beginRedrawTimePointCPU);
			Logger::AddRedrawTime(duration_cast<milliseconds>(finishedRedrawTimePoint - beginRedrawTimePoint));
			timerRunning = false;
		}
		drawingStatusStr << "Tasks Empty, Threads Sleeping";
	}
		drawingStatusText.setString(drawingStatusStr.str());
}

void DrawingManager::DrawGUI()
{
	//Display controls
	window->draw(controlsText);

	//Display if all threads done
	window->draw(drawingStatusText);
}

bool DrawingManager::IsDone() const
{
	return isDone;
}

void DrawingManager::ProcessInput()
{
	bool somethingPressed = false;
	//zoom in and out
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add))
	{
		Mandelbrot::TotalRect.Scale *= zoomAmount; //mimic x^2 so zoom portion appears to be identical no matter how far zoomed in

		somethingPressed = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Dash) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract))
	{
		Mandelbrot::TotalRect.Scale /= zoomAmount;

		somethingPressed = true;
	}

	//move left and right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
	{
		Mandelbrot::TotalRect.Translation.x -= moveAmount * Mandelbrot::TotalRect.Scale;

		somethingPressed = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
	{
		Mandelbrot::TotalRect.Translation.x += moveAmount * Mandelbrot::TotalRect.Scale;

		somethingPressed = true;
	}

	//move up and down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
	{
		Mandelbrot::TotalRect.Translation.y += moveAmount * Mandelbrot::TotalRect.Scale;

		somethingPressed = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
	{
		Mandelbrot::TotalRect.Translation.y -= moveAmount * Mandelbrot::TotalRect.Scale;

		somethingPressed = true;
	}

	//Reset viewport
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		Mandelbrot::TotalRect.Reset();

		somethingPressed = true;
	}

	//Exit
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		window->close();

	if (somethingPressed)
	{
		Mandelbrot::ITERATIONS = Mandelbrot::START_ITERATIONS / Mandelbrot::TotalRect.Scale; //increase number of iterations to keep the zoomed in areas highly detailed
		Redraw();
	}
}