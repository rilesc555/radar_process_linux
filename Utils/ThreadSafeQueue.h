#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H


#include <mutex>
#include <condition_variable>
#include <queue>
#include <exception>
#include <iostream>


template <typename T>
class ThreadSafeQueue {
private:
	std::queue<T> queue;
	mutable std::mutex mtx;
	std::condition_variable cvNotEmpty;
	bool finished = false;

public:
	ThreadSafeQueue() = default;
	ThreadSafeQueue(const ThreadSafeQueue& other) = delete;
	ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete;

	void enqueue(T value) {
		std::lock_guard<std::mutex> lock(mtx);
		queue.push(std::move(value));
		cvNotEmpty.notify_one();
	}
	T dequeue() {
		std::unique_lock<std::mutex> lock(mtx);
		cvNotEmpty.wait(lock, [this] { return !queue.empty(); });
		T value = std::move(queue.front());
		queue.pop();
		return value;
	}
	bool empty() {
		std::lock_guard<std::mutex> lock(mtx);
		return queue.empty();
	}
	size_t size() {
		std::lock_guard<std::mutex> lock(mtx);
		return queue.size();
	}
	void setFinished() {
		finished = true;
	}
	bool getFinished() {
		return finished;
	}
};

#endif