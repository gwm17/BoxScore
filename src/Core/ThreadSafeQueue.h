#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <deque>
#include <mutex>
#include <thread>

namespace BoxScore {

	template<typename T>
	class ThreadSafeQueue
	{
	public:
		ThreadSafeQueue() = default;
		ThreadSafeQueue(const ThreadSafeQueue&) = delete; //no copy
		~ThreadSafeQueue() { Clear(); }

		void PushBack(const T& data)
		{
			std::scoped_lock<std::mutex> guard(m_queueMutex);
			m_queue.push_back(data);
		}

		void PushFront(const T& data)
		{
			std::scoped_lock<std::mutex> guard(m_queueMutex);
			m_queue.push_front(data);
		}

		void PopBack()
		{
			std::scoped_lock<std::mutex> guard(m_queueMutex);
			m_queue.pop_back();
		}

		void PopFront()
		{
			std::scoped_lock<std::mutex> guard(m_queueMutex);
			m_queue.pop_front();
		}

		const T& Front()
		{
			std::scoped_lock<std::mutex> guard(m_queueMutex);
			return m_queue.front();
		}

		const T& Back()
		{
			std::scoped_lock<std::mutex> guard(m_queueMutex);
			return m_queue.back();
		}

		std::size_t Size()
		{
			std::scoped_lock<std::mutex> guard(m_queueMutex);
			return m_queue.size();
		}

		bool IsEmpty()
		{
			std::scoped_lock<std::mutex> guard(m_queueMutex);
			return m_queue.empty();
		}

		void Clear()
		{
			std::scoped_lock<std::mutex> guard(m_queueMutex);
			m_queue.clear();
		}

		//For iterator loops, need begin()/end() idiom

		std::deque<T>::iterator begin()
		{
			std::scoped_lock<std::mutex> guard(m_queueMutex);
			return m_queue.begin();
		}

		std::deque<T>::iterator end()
		{
			std::scoped_lock<std::mutex> guard(m_queueMutex);
			return m_queue.end();
		}

	private:
		std::mutex m_queueMutex;
		std::deque<T> m_queue;
	};
}

#endif