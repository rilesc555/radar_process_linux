//*****************************************************************************
// thread_helper.h - Objecst that make threading easier tbh
//
// Author: Bryan Womack
// Date: October 18, 2017
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef THREAD_HELPER_H
#define THREAD_HELPER_H

#include <pthread.h>
#include <semaphore.h>
#include <mutex>
#include "BNET_Exception.h"

class d_flag
{
	bool flag;
	std::timed_mutex m_mutex;

	public:
		d_flag()
		{
			flag = false;
		}

		~d_flag()
		{

			// Note, since we switched to std::mutex it will destroy itself
		}

		bool get(void) { return flag; };
		void set(bool val) { flag = val; };
		void lock(void) { m_mutex.lock(); };
		void unlock(void) { m_mutex.unlock(); };

		std::unique_lock<std::timed_mutex> unique_lock(void)
		{
			std::chrono::seconds sec(10);
			std::unique_lock<std::timed_mutex> lock(m_mutex, sec);
			if (!lock.owns_lock())
			{
				throw BNET_Exception("BNET is in a deadlock.");
			}
			return lock;
		}

		bool protected_get(void)
		{
			std::unique_lock<std::timed_mutex> lock(m_mutex);

			return flag;
		}

		void protected_set(bool val)
		{
			std::unique_lock<std::timed_mutex> lock(m_mutex);

			flag = val;
		}
};

class prco_helper
{
	pthread_mutex_t mutex;
	pthread_cond_t more;
	pthread_cond_t less;
	int			capacity;
	int			occupied;
	int			q_in;
	int			q_out;

	public:
		prco_helper(int capacity_)
		{
			pthread_mutex_init(&mutex, NULL);
			pthread_cond_init(&more, NULL);
			pthread_cond_init(&less, NULL);

			occupied = 0;
			q_in = 0;
			q_out = 0;
			this->capacity = capacity_;
		}

		~prco_helper(void)
		{
			pthread_mutex_destroy(&mutex);
			pthread_cond_destroy(&more);
			pthread_cond_destroy(&less);
		}

		int
		get_in_index(void)
		{
			pthread_mutex_lock(&mutex);
			while (occupied >= capacity)
			{
				pthread_cond_wait(&less, &mutex);
			}

			++q_in;
			q_in %= capacity;
			++occupied;

			if (occupied <= 0 || occupied > capacity)
			{
				pthread_mutex_unlock(&mutex);
				return -1; // TODO: Go apeshit
			}

			return q_in;
		}

		void
		unlock_in_index(void)
		{
			pthread_cond_signal(&more);
			pthread_mutex_unlock(&mutex);
		}

		int
		get_out_index(void)
		{
			pthread_mutex_lock(&mutex);
			while(occupied <= 0)
			{
				pthread_cond_wait(&more, &mutex);
			}

			if (occupied <= 0 || occupied > capacity)
			{
				pthread_mutex_unlock(&mutex);
				return -1; // TODO: Go apeshit
			}

			++q_out;
			q_out %= capacity;
			--occupied;

			return q_out;
		}

		void
		unlock_out_index(void)
		{
			pthread_cond_signal(&less);
			pthread_mutex_unlock(&mutex);
		}

		void
		block_until_empty(void)
		{
			bool is_occupied;

			do
			{
				pthread_mutex_lock(&mutex);
				is_occupied = occupied > 0;
				pthread_mutex_unlock(&mutex);
			} while(is_occupied);
		}

};

class d_thread
{
	d_flag quit;
	pthread_t thread_handle;

	public:
		d_thread()
		{
            thread_handle = 0;
		}

		~d_thread()
		{
			end();
		};

		void start(void *(*start_routine) (void *), void* arg)
		{
			pthread_create(&thread_handle, NULL, start_routine, arg);
		};

		void set_quit(bool val)
		{
			quit.protected_set(val);
		}

		void join(void)
		{
            if (thread_handle != 0)
            {
                pthread_join(thread_handle, NULL);
                thread_handle = 0;
            }
		}

		void end(void)
		{
			set_quit(true);
			join();
			set_quit(false);
		};

		bool quit_request(void)
		{
			return quit.protected_get();
		};
};

#endif /* THREAD_HELPER_H */

/* EOF */
