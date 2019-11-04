#include "Logger.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::string Logger::totalRedrawTimes = "";
std::string Logger::tileDrawTimes = "";
std::string Logger::cpuTotalRedrawTimes = "";

std::mutex Logger::addTileTimeMutex;

int Logger::CurrentRun = 0;
int Logger::Segments = 0;

bool Logger::IsBenchmarking = false;

void Logger::AddHeaders()
{
	totalRedrawTimes += "Run, Segments, Total Redraw Time (ms)\n";
	tileDrawTimes += "Tile, Iterations, Segments, Run, Mandelbrot Time (ms)\n";
	cpuTotalRedrawTimes += "Run, Segments, Total Redraw CPU Time (ms)\n";
}

void Logger::AddRedrawTime(timeunit redrawTime)
{
	if (IsBenchmarking)
	{
		totalRedrawTimes += std::to_string(CurrentRun);
		totalRedrawTimes += ",";

		totalRedrawTimes += std::to_string(Segments);
		totalRedrawTimes += ",";
	
		totalRedrawTimes += std::to_string(redrawTime.count());
		totalRedrawTimes += "\n";
	}
}

void Logger::AddTileTime(const sf::Vector2i& tilePos, const int iterations, timeunit redrawTime)
{
	if (LogTiles && IsBenchmarking)
	{
		std::unique_lock<std::mutex> lock(addTileTimeMutex); //Affects Totaltime but it prevents threads writing to the same string at the same time
	
		tileDrawTimes += std::to_string(tilePos.x);
		tileDrawTimes += ".";
		tileDrawTimes += std::to_string(tilePos.y);
		tileDrawTimes += ",";

		tileDrawTimes += std::to_string(iterations);
		tileDrawTimes += ",";
	
		tileDrawTimes += std::to_string(Segments);
		tileDrawTimes += ",";
	
		tileDrawTimes += std::to_string(CurrentRun);
		tileDrawTimes += ",";
	
		tileDrawTimes += std::to_string(redrawTime.count());
		tileDrawTimes += "\n";
	}
}

void Logger::AddCPUTime(const std::clock_t time)
{
	if (IsBenchmarking)
	{
		cpuTotalRedrawTimes += std::to_string(CurrentRun);
		cpuTotalRedrawTimes += ",";

		cpuTotalRedrawTimes += std::to_string(Segments);
		cpuTotalRedrawTimes += ",";

		cpuTotalRedrawTimes += std::to_string(time);
		cpuTotalRedrawTimes += "\n";
	}
}

void Logger::WriteStringToAll(const std::string str)
{
	totalRedrawTimes += str;
	tileDrawTimes += str;
	cpuTotalRedrawTimes += str;
}

void Logger::SaveAll(std::string appendOnEnd)
{
	if (IsBenchmarking)
	{
		std::stringstream str1;
		str1 << "Total Redraw Times" << appendOnEnd << ".csv";
		saveFile(totalRedrawTimes, str1.str());

		if (LogTiles)
		{
			std::stringstream str2;
			str2 << "Tile Draw Times" << appendOnEnd << ".csv";
			saveFile(tileDrawTimes, str2.str());
		}
		
		std::stringstream str3;
		str3 << "Total Redraw CPU Times" << appendOnEnd << ".csv";
		saveFile(cpuTotalRedrawTimes, str3.str());
	}
}

void Logger::ResetContentsAndAddHeaders()
{
	totalRedrawTimes = "";
	tileDrawTimes = "";
	cpuTotalRedrawTimes = "";

	AddHeaders();
}

void Logger::saveFile(std::string & str, std::string filename)
{
	std::fstream file;
	file.open(filename, std::ios::out | std::ios::trunc);
	if (file.is_open())
	{
		file << str;
		file.close();
		std::cout << "Saved logs to " << filename << ".\n";
	}
	else
	{
		std::cerr << "Failed to open " << filename << " for writing.\n";
	}
}
