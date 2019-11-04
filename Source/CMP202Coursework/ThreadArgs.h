#pragma once

#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>
#include <atomic>

#include "Task.h"

/// <summary>
/// For holding thread arguments.
/// </summary>
struct ThreadArgs
{
	std::queue<Task*>* tasks;
	std::mutex * tasksQueueMutex;
	bool* endThread;
	std::atomic<bool>* isThreadSleeping;
	std::atomic<bool>* wakeUp;
	std::mutex* wakeUpThreadMutex;
	std::condition_variable* threadDoneCV;

	/// <summary>
	/// A container struct for thread arguments.
	/// </summary>
	/// <param name="tasks">Pointer to tasks queue.</param>
	/// <param name="tasksQueueMutex">Pointer to the mutex for the task queue.</param>
	/// <param name="endThread">Pointer to boolean which determines whether the thread should exit.</param>
	/// <param name="wakeUp">Pointer to boolean which determines if the thread should wake up.</param>
	/// <param name="wakeUpThreadMutex">Mutex to previous boolean.</param>
	/// <param name="threadDoneCV">Condition variable for making the thread wait until notified when its asleep.</param>
	/// <param name="isThreadSleeping">A bool that is set to true when the thread is asleep, and set to false when its not.</param>
	ThreadArgs(std::queue<Task*>* tasks, std::mutex * tasksQueueMutex, bool* endThread, std::atomic<bool>* wakeUp, std::mutex* wakeUpThreadMutex, std::condition_variable* threadDoneCV, std::atomic<bool>* isThreadSleeping)
		: tasks(tasks),
		tasksQueueMutex(tasksQueueMutex),
		endThread(endThread),
		wakeUp(wakeUp),
		wakeUpThreadMutex(wakeUpThreadMutex),
		threadDoneCV(threadDoneCV),
		isThreadSleeping(isThreadSleeping)
	{}
	~ThreadArgs() {}
};