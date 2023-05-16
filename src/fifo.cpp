#include "fifo.h"
#include <unistd.h>

FIFO::FIFO(const std::string &filename, const int &num_tables)
{
    read_workload(filename);
    this->xs = get_job_queue();
    this->num_tables = num_tables;
    for (int i = 0; i < num_tables; i++)
    {
        this->threads.emplace_back(&FIFO::run_policy, this);
    }
    sleep(1); // sleep for 1 second to wait for all threads to initialize
    cout_lock.lock();
    std::cout << "\n**All threads initialized.. beginning work**\n" << std::endl;
    cout_lock.unlock();
    pthread_cond_broadcast(&cond);
}

void FIFO::run_policy()
{
    Metrics thread_metrics;

    pthread_mutex_lock(&mutex);
    std::cout << "Thread " << pthread_self() << ": initialized. Waiting for all threads to be ready." << std::endl;
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex); // unlocking for all other threads

    pthread_mutex_lock(&queue_mutex);
    while (!xs.empty())
    {
        pthread_mutex_unlock(&queue_mutex);

        /**
         * Get job from job queue
         */
        // continue popping customers until the current time is less than their arrival time + willingness to wait
        pthread_mutex_lock(&queue_mutex);
        while (!xs.empty() && xs.top().arrival + xs.top().willingness_to_wait <= time)
        {
            cout_lock.lock();
            std::cout << "Thread " << pthread_self() << ": dropping customer " << xs.top().arrival << " at time " << time << std::endl;
            cout_lock.unlock();
            xs.pop();
            pthread_mutex_unlock(&queue_mutex);
            if (num_tables > 1)
                sleep(1);
            pthread_mutex_lock(&queue_mutex);
        }
        pthread_mutex_unlock(&queue_mutex);

        // if the queue is empty after dropping customers, break out of the loop
        pthread_mutex_lock(&queue_mutex);
        if (xs.empty())
        {
            break;
        }
        Customer p = xs.top();
        xs.pop();
        pthread_mutex_unlock(&queue_mutex); // unlock the thread

        /**
         * Update metrics
         */
        pthread_mutex_lock(&time_mutex);
        if (time < p.arrival)
        {
            time = p.arrival;
        }
        p.first_run = time;
        time += p.duration;
        p.completion = time;
        pthread_mutex_unlock(&time_mutex);

        /**
         * Update completed jobs
         */
        pthread_mutex_lock(&completed_jobs_mutex); // lock the thread
        cout_lock.lock();
        std::cout << "Thread " << pthread_self() << ": completing customer " << p.arrival << " at time " << p.completion << std::endl;
        cout_lock.unlock();
        completed_jobs.push_back(p);
        pthread_mutex_unlock(&completed_jobs_mutex); // unlock the thread
        if (num_tables > 1)
            sleep(1);                                    // short sleep to let other threads run
        pthread_mutex_lock(&queue_mutex); // lock the thread
    }
    pthread_mutex_unlock(&queue_mutex);
    return;
}