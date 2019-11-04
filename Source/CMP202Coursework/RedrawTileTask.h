#pragma once
#include "Task.h"
#include "Tile.h"

class RedrawTileTask :
	public Task
{
private:
	Tile * tile;

public:
	RedrawTileTask(Tile * tile);
	~RedrawTileTask();

	// Inherited via Task
	virtual void run() override;
};
