# Interactive-Mandelbrot
An interactive Mandelbrot using a thread farm/pool made using SFML and STL threads.

![GIF of Interactive Mandelbrot running](https://github.com/giodestone/Interactive-Mandelbrot/blob/master/Images/GIF.gif)

# How to use
WSAD, Arrow Keys, or Numpad 8, 6, 4, 2 to move;
+ and - (including Numpad) to zoom;
R to reset view.

# How it works
The screen is divided up into tiles. Each of them is re-rendered when input happens.

The rendering is done on the CPU using a thread pool. Each of the threads gets a task allocated.

Key tasks are:
* Redraw tile (render it again with new paramaters)
* 'Desaturate tile' (Dim the tile to tell the user that it's out of date)

# More Images
![Zoomed part of the Mandelbrot set](https://github.com/giodestone/Interactive-Mandelbrot/blob/master/Images/Zoomed Section.png)
![Home Page of application](https://github.com/giodestone/Interactive-Mandelbrot/blob/master/Images/Home Page.png)
![Another zoomed part of the Mandelbrot Set](https://github.com/giodestone/Interactive-Mandelbrot/blob/master/Images/Zoomed 2.png)
![Rendering a scene](https://github.com/giodestone/Interactive-Mandelbrot/blob/master/Images/Rendering.png)

# Implementation Notes
Uses SFML to not spend time implementing repetitive things.

## Inter thread communication
Signalling the thread to wake up is done using a condition variable.

Mutexes are used for the queue.

## Number of threads
The number of threads in minimum two, but otherwise automatically set to the amount of cores the operating system sees.

## Drawing Manager
The Drawing Manager class handles drawing, input, and scheduling.

## Mandelbrot static class
The Mandelbrot static class contains functions. Made static as it doesn't need to be instanced.

## Logger Class
The logger class is for taking notes of time in CSV's, which can later be made into graphs.

### Running benchmarks
Uncomment RunStatistics in the main function. This probably should've been made a mode.
