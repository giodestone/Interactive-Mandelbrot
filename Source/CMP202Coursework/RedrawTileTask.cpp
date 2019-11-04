#include "RedrawTileTask.h"

RedrawTileTask::RedrawTileTask(Tile * tile) : tile(tile)
{}

RedrawTileTask::~RedrawTileTask()
{}

void RedrawTileTask::run()
{
	tile->RedrawTile();
}