#pragma once

#include <functional>

#include <algorithm>
#include <atomic>
#include <thread>
#include <condition_variable>

#include "ThreadSafeRingBuffer.h"

struct JobDispatchArgs
{
	uint32_t jobIndex;
	uint32_t groupIndex;
};

class JobSystem
{
public:
	void Initialize();

	void Execute(const std::function<void()>& job);
	void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job);
	void Wait();

	void Poll();

	bool IsBusy();

private:
	uint32_t numThreads;

	ThreadSafeRingBuffer<std::function<void()>, 256> jobPool;

	std::condition_variable wakeCondition;

	std::mutex wakeMutex;

	uint64_t currentLabel = 0;

	std::atomic<uint64_t> finishedLabel;
};