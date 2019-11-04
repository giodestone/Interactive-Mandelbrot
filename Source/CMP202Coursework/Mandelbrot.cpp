#include "Mandelbrot.h"

#include <cstdint>
#include <cstdlib>
#include <complex>
#include <fstream>
#include <iostream>
#include <thread>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::complex;
using std::cout;
using std::endl;
using std::ofstream;

int Mandelbrot::START_ITERATIONS = 50;
int Mandelbrot::ITERATIONS = START_ITERATIONS;
sf::RenderWindow* Mandelbrot::window = nullptr;
MandelbrotRectangle<double> Mandelbrot::TotalRect(-2.0, 1.0, 1.125, -1.125, 1.0);

void Mandelbrot::compute_mandelbrot(MandelbrotRectangle<double>& totalRect, sf::IntRect& segment, sf::Image* image)
{
	for (int y = segment.top; y < segment.top + segment.height; ++y)
	{
		for (int x = segment.left; x < segment.left + segment.width; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<double> c(totalRect.GetTranslated().left + (x * (totalRect.GetTranslated().width - totalRect.GetTranslated().left) / window->getSize().x),
								totalRect.GetTranslated().top + (y * (totalRect.GetTranslated().height - totalRect.GetTranslated().top) / window->getSize().y));

			// Start off z at (0, 0).
			complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			while (abs(z) < 2.0 && iterations < ITERATIONS)
			{
				z = (z * z) + c;

				++iterations;
			}
			// z didn't escape from the circle.
			// This point is in the Mandelbrot set.
			image->setPixel(x - segment.left, y - segment.top, get_iteration_color(iterations)); //take away because the image needs to set an individual pixel
		}
	}
}

sf::Color Mandelbrot::get_iteration_color(int currentIteration)
{
	if (currentIteration < ITERATIONS / 2)
		return sf::Color(0,
						static_cast<int>(((static_cast<float>(currentIteration) / static_cast<float>(ITERATIONS)) * 255.f)),
						0);
	else
		return sf::Color(static_cast<int>(((static_cast<float>(currentIteration) / static_cast<float>(ITERATIONS)) * 255.f)),
					0,
					0);
}
