#include "Farm.h"

using std::thread;
using std::cout;
using std::unique_lock;
using std::mutex;

Farm::Farm(unsigned int noOfThreads = 0) :
	endThreads(false),
	wakeUpThreads(false)
{
	if (noOfThreads == 0)
	{
		if (std::thread::hardware_concurrency() < 2) //create atleast two worker threads (+1 main)
			noOfThreads = 1;
		else
			noOfThreads = std::thread::hardware_concurrency();
	}

	threads.reserve(noOfThreads); //little optimisation to make sure vector doesn't get too big
	threadsSleeping.reserve(noOfThreads);

	for (unsigned int i = 0; i < noOfThreads; i++)
	{
		std::atomic<bool>* currentAtomic = new std::atomic<bool>;
		currentAtomic->store(false);

		ThreadArgs args(&tasks, &tasksQueueMutex, &endThreads, &wakeUpThreads, &wakeUpThreadsMutex, &wakeUpThreadsCV, currentAtomic);
		threadsSleeping.push_back(currentAtomic);

		thread t(Farm::worker, args);
		threads.push_back(std::move(t));
	}
}

Farm::~Farm()
{
	endThreads = true;
	wakeUpThreads = true;
	wakeUpThreadsCV.notify_all();

	//delete any remaining tasks
	ClearTasks();

	//join all threads
	bool anyNonJoinable = true;
	while (anyNonJoinable)
	{
		wakeUpThreadsCV.notify_all();
		anyNonJoinable = false;
		for (auto& thread : threads)
		{
			if (thread.joinable())
				thread.join();
			else
				anyNonJoinable = true;
		}
	
	}

	//delete all atomics
	for (auto it = threadsSleeping.begin(); it != threadsSleeping.end();)
	{
		delete *it;
		*it = nullptr;
		it = threadsSleeping.erase(it);
	}

}

void Farm::AddTask(Task *task)
{
	std::unique_lock<std::mutex> lock(tasksQueueMutex);
	tasks.push(task);
}

void Farm::ClearTasks()
{
	std::unique_lock<mutex> lock(tasksQueueMutex);

	while (!tasks.empty())
	{
		delete tasks.front();
		tasks.front() = nullptr;
		tasks.pop();
	}
}

void Farm::Run()
{
	/* This method was primarly created because without the constant notifying of the threads they wouldn't wake up.*/

	//stop the threads being me before 9am's
	std::unique_lock<mutex> lock(tasksQueueMutex);
	if (!tasks.empty()) //when there are tasks make the threads poll for new tasks
	{
		wakeUpThreads.store(true);
		wakeUpThreadsCV.notify_all();
	}
	else //otherwise let them doze off
	{
		wakeUpThreads.store(false);
	}
}

bool Farm::IsTaskAvaliable()
{
	std::unique_lock<mutex> lock(tasksQueueMutex);
	return !tasks.empty();
}

bool Farm::IsAllThreadsAsleep()
{
	for (auto& atomic : threadsSleeping)
	{
		if (atomic->load() == false)
			return false;
	}
	return true;
}

void Farm::worker(ThreadArgs threadArgs)
{
	while (!(*threadArgs.endThread))
	{
		threadArgs.tasksQueueMutex->lock();
		if (!threadArgs.tasks->empty()) //check if theres anything to do
		{
			//get task at front
			Task* currentTask = nullptr;
			currentTask = threadArgs.tasks->front();
			threadArgs.tasks->pop();
			threadArgs.tasksQueueMutex->unlock();

			//run the task after unlocking mutex
			auto currentThreadId = std::this_thread::get_id();
			currentTask->run();
			delete currentTask; //delete it
			currentTask = nullptr;
		}
		else
		{
			threadArgs.tasksQueueMutex->unlock();

			std::unique_lock<mutex> lock(*threadArgs.wakeUpThreadMutex);

			(*threadArgs.isThreadSleeping) = true; //says this thread is asleep

			//std::cout << "Thread " << std::this_thread::get_id() << " waiting...\n";

			while (!threadArgs.wakeUp->load()) //block execution until threads are woken up
				threadArgs.threadDoneCV->wait(lock);

			(*threadArgs.isThreadSleeping) = false;

			//std::cout << "Thread " << std::this_thread::get_id() << " woken up!\n";
		}
	}
}