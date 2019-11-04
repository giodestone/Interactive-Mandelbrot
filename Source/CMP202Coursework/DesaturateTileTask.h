#pragma once
#include "Task.h"

#include "Tile.h"
class DesaturateTileTask :
	public Task
{
	Tile* tile;

public:
	DesaturateTileTask(Tile * tile);
	~DesaturateTileTask();

	// Inherited via Task
	virtual void run() override;
};
