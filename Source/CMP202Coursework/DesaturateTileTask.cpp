#include "DesaturateTileTask.h"

DesaturateTileTask::DesaturateTileTask(Tile * tile)
	: tile(tile)
{}

DesaturateTileTask::~DesaturateTileTask()
{}

void DesaturateTileTask::run()
{
	tile->setColor(sf::Color(128, 0, 128));
}