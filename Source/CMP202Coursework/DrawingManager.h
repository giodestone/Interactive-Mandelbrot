#pragma once

#include "Tile.h"
#include "Farm.h"

#include "Mandelbrot.h"

#include <chrono>

/// <summary>
/// Takes care of drawing, redrawing elements and also contains the task farm.
/// </summary>
class DrawingManager
{
	sf::RenderWindow* window;
	unsigned int segments;

	std::vector<Tile> tiles;

	Farm taskFarm;

	sf::Font font;
	sf::Text controlsText;
	sf::Text drawingStatusText;

	float zoomAmount;

	float moveAmount;

	//Benchmarking
	bool timerRunning;
	std::chrono::steady_clock::time_point beginRedrawTimePoint;
	std::chrono::steady_clock::time_point finishedRedrawTimePoint;

	std::clock_t beginRedrawTimePointCPU; //For CPU Time
	std::clock_t finishedRedrawTimePointCPU; //For CPU Time

	bool isDone;

public:
	static constexpr int GUIHeight = 40;

	DrawingManager(sf::RenderWindow* window, unsigned int segments, int startIterations);
	~DrawingManager();

	/// <summary>
	/// Call redraw all tiles, cancel any ones.
	/// </summary>
	void Redraw();

	/// <summary>
	/// Run nessesary functions to keep task farm function and any updates to drawing.
	/// </summary>
	void Run();

	/// <summary>
	/// Process any input.
	/// </summary>
	void ProcessInput();

	/// <summary>
	/// Draw all of the tiles to the screen.
	/// </summary>
	/// <remarks>GUIHeight is left blank, this is where all of the GUI elements should be drawn to.</remarks>
	void DrawTiles();

	/// <summary>
	/// Draw any GUI elements.
	/// </summary>
	void DrawGUI();

	/// <summary>
	/// Is computation done.
	/// </summary>
	/// <returns>True if the drawing is done.</returns>
	bool IsDone() const;
};
