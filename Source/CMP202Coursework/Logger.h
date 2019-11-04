#pragma once
#include <string>
#include <chrono>
#include <mutex>
#include <ctime>
#include <SFML/System/Vector2.hpp>

class Logger
{
	typedef std::chrono::milliseconds timeunit;

	static std::string totalRedrawTimes;
	static std::string tileDrawTimes;
	static std::string cpuTotalRedrawTimes;
	
public:
	static constexpr bool LogTiles = false;
	static int CurrentRun; //For keeping track of the current run.
	static int Segments;
	static bool IsBenchmarking;

	/// <summary>
	/// Add the field titles to the start of the files.
	/// </summary>
	static void AddHeaders();

	/// <summary>
	/// Add a timer of how long a total redraw took.
	/// </summary>
	/// <param name="redrawTime">The time that it took to redraw.</param>
	static void AddRedrawTime(timeunit redrawTime);

	/// <summary>
	/// Adds a value to the tile time.
	/// </summary>
	/// <param name="tilePos">The position of the tile.</param>
	/// <param name="iterations">How many iterations were carried out.</param>
	/// <param name="redrawTime">How long did the redraw take.</param>
	/// <remarks>Threadsafe.</remarks>
	static void AddTileTime(const sf::Vector2i& tilePos, const int iterations, timeunit redrawTime);

	/// <summary>
	/// Add a CPU time.
	/// </summary>
	/// <param name="time">The time taken between the start and end of measurement (end - start).</param>
	static void AddCPUTime(const std::clock_t time);

	/// <summary>
	/// Write a specific string to all strings held internally.
	/// </summary>
	/// <param name="str">String to append onto the end of the internal strings.</param>
	static void WriteStringToAll(const std::string str);

	/// <summary>
	/// Saves all internal strings.
	/// </summary>
	/// <param name="appendOnEnd">What to add before the extension but after the initialFileName</param>
	static void SaveAll(std::string appendOnEnd="");

	/// <summary>
	/// Resets the contents of all internal strings and adds headers.
	/// </summary>
	static void ResetContentsAndAddHeaders();

private:
	/// <summary>
	/// Saves a string to a file.
	/// </summary>
	/// <param name="str">String to save.</param>
	/// <param name="filename">File name plus extension (.csv).</param>
	static void saveFile(std::string& str, std::string filename);
	static std::mutex addTileTimeMutex;

	Logger() {}
	~Logger() {}
};