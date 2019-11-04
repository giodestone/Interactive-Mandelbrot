#pragma once

#include "ThreadArgs.h"

#include <mutex>
#include <queue>
#include <vector>
#include <thread>
#include <iostream>

#include <future>
#include <condition_variable>
#include <atomic>

/// <summary>
/// Implementation of a task farm, with thread sleeping/waking functionality.
/// </summary>
class Farm
{
	std::queue<Task *> tasks;
	std::mutex tasksQueueMutex;

	std::vector<std::thread> threads;
	std::vector<std::atomic<bool>*> threadsSleeping;
	bool endThreads;

	std::atomic<bool> wakeUpThreads;
	std::mutex wakeUpThreadsMutex;
	std::condition_variable wakeUpThreadsCV;

public:
	/// <summary>
	/// How many threads should be ran.
	/// </summary>
	/// <param name="noOfThreads">Amount of worker threads. If set to 0/no arguments it becomes atleast two, with a max of how many CPUs the machiene has -
	/// based off of std::thread::hardware_concurrency().</param>
	Farm(unsigned int noOfThreads);
	~Farm();

	/// <summary>
	/// Add a task to be processed.
	/// </summary>
	/// <param name="task">The task to add onto the queue.</param>
	/// <remarks>Threadsafe.</remarks>
	void AddTask(Task *task);

	/// <summary>
	/// Clears all tasks.
	/// </summary>
	void ClearTasks();

	/// <summary>
	/// Performs operations to keep the task farm running, such as making the sleep thread wait.
	/// Designed to be called once every frame.
	/// </summary>
	void Run();

	/// <summary>
	/// Returns whether the threads should be awake or not.
	/// </summary>
	/// <returns>Whether the threads should be awake or not.</returns>
	/// <remarks>Threadsafe.</remarks>
	bool IsTaskAvaliable();

	/// <summary>
	/// Checks if all threads are asleep.
	/// </summary>
	/// <returns>True if all threads are asleep.</returns>
	/// <remarks>Threadsafe.</remarks>
	bool IsAllThreadsAsleep();

private:
	/// <summary>
	/// Worker function for the thread.
	/// </summary>
	/// <param name="threadArgs">Arguments for the thread. See ThreadArgs.</param>
	static void worker(ThreadArgs threadArgs);
};