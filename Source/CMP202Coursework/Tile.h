#pragma once

#include <SFML\Graphics.hpp>

#include "Logger.h"
#include "Mandelbrot.h"

/// <summary>
/// Tile stores what area of the mandelbrot should be calculates as well as the memory where it should be drawn to.
/// </summary>
/// <seealso cref="RedrawTileTask"/>
class Tile : public sf::Sprite
{
	sf::Vector2i segmentCoord;
	int iterations;

	sf::IntRect segment;
	sf::Texture texture;

	sf::Image image;

public:
	/// <summary>
	/// Tile stores what area of the mandelbrot should be calculates as well as the memory where it should be drawn to.
	/// </summary>
	/// <param name="segment">The pixels which should be drawn</param>
	/// <param name="segmentCoord">The coordinates for the tile relative to others. This is used for logging only.</param>
	/// <param name="iterations">The amount of iterations being caluclated. This is used for logging only.</para>
	Tile(sf::IntRect segment, sf::Vector2i segmentCoord, int iterations);
	~Tile();

	void RedrawTile();
};
