#pragma once
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <complex>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <SFML\Graphics.hpp>

#include "MandelbrotRectangle.h"

class Mandelbrot
{
public:
	static int START_ITERATIONS;

	static int ITERATIONS;

	//Pointer to window to get window size.
	static sf::RenderWindow* window;

	//For storing the values for the compute_mandelbrot function.
	static MandelbrotRectangle<double> TotalRect;

	/// <summary>
	/// Computes mandelbrot set for said pixel.
	/// </summary>
	/// <param name="totalRect">The total size of the window.</param>
	/// <param name="segment">Which segment should be rendered.</param>
	/// <param name="image">The image that it should be outputted to.</param>
	static void compute_mandelbrot(MandelbrotRectangle<double>& totalRect, sf::IntRect& segment, sf::Image* image);

	static sf::Color get_iteration_color(int currentIteration);

private:
	Mandelbrot() {}
	~Mandelbrot() {}

};

