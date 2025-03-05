#pragma once

#include <Net/NetCommon.h>

namespace arc
{
	namespace net
	{
		template<typename T>
		class TSQueue
		{
		public:
			TSQueue() = default;
			TSQueue(const TSQueue<T>&) = delete;
			virtual ~TSQueue() { clear(); }

		public:
			const T& front()
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.front();
			}

			const T& back()
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.back();
			}

			const T& pushback(const T& item)
			{
				std::scoped_lock lock(muxQueue);
				deqQueue.emplace_back(std::move(item));
			}

			const T& pushfront(const T& item)
			{
				std::scoped_lock lock(muxQueue);
				deqQueue.emplace_front(std::move(item));
			}

			bool empty()
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.empty();
			}

			size_t count()
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.size();
			}

			void clear()
			{
				std::scoped_lock lock(muxQueue);
				deqQueue.clear();
			}

			T popfront()
			{
				std::scoped_lock lock(muxQueue);
				auto T = std::move(deqQueue.front());
				deqQueue.pop_front();
				return T;
			}

			T popback()
			{
				std::scoped_lock lock(muxQueue);
				auto T = std::move(deqQueue.back());
				deqQueue.pop_back();
				return T;
			}

		protected:
			std::mutex muxQueue;
			std::deque<T> deqQueue;
		};
	}
}