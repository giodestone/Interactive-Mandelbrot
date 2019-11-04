#include "Tile.h"

#include <chrono>

Tile::Tile(sf::IntRect segment, sf::Vector2i segmentCoord, int iterations)
	: segment(segment),
	segmentCoord(segmentCoord),
	iterations(iterations)
{
	image.create(segment.width, segment.height, sf::Color(rand() % 255, rand() % 255, rand() % 255, 127));
	//texture.loadFromImage(image);
	//setTexture(texture);
	setPosition(segment.left, segment.top);
}

Tile::~Tile()
{}

void Tile::RedrawTile()
{
	if (Logger::LogTiles)
	{
		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

		Mandelbrot::compute_mandelbrot(Mandelbrot::TotalRect, segment, &image);

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		Logger::AddTileTime(segmentCoord, iterations, std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
	}
	else
	{
		Mandelbrot::compute_mandelbrot(Mandelbrot::TotalRect, segment, &image);
	}

	setColor(sf::Color::White); //reset colour incase it has been desaturated.
	texture.loadFromImage(image); //need to reload into GPU memory
	setTexture(texture);
}