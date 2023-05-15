#include "sjf.h"
#include <unistd.h>

SJF::SJF(const std::string &filename, const int &num_tables)
{
    read_workload(filename);
    this->xs = get_job_queue();
    this->num_tables = num_tables;
    for (int i = 0; i < num_tables; i++)
    {
        this->threads.emplace_back(&SJF::run_policy, this);
    }
    sleep(1); // sleep for 1 second to wait for all threads to initialize
    cout_lock.lock();
    std::cout << "\n**All threads initialized.. beginning work**\n"
              << std::endl;
    cout_lock.unlock();
    pthread_cond_broadcast(&cond);
}

void SJF::run_policy()
{
    Metrics thread_metrics;

    pthread_mutex_lock(&mutex);
    std::cout << "Thread " << pthread_self() << ": initialized. Waiting for all threads to be ready." << std::endl;
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex); // unlocking for all other threads

    pthread_mutex_lock(&queue_mutex);
    while (!xs.empty() || !ys.empty())
    {
        pthread_mutex_unlock(&queue_mutex); // unlock the thread

        // continue popping customers until the current time is less than their arrival time + willingness to wait
        pthread_mutex_lock(&queue_mutex);
        while (!xs.empty() && xs.top().arrival + xs.top().willingness_to_wait <= thread_metrics.time_elapsed)
        {
            cout_lock.lock();
            std::cout << "Thread " << pthread_self() << ": dropping customer " << xs.top().arrival << " at time " << thread_metrics.time_elapsed << std::endl;
            cout_lock.unlock();
            xs.pop();
            pthread_mutex_unlock(&queue_mutex);
            if (num_tables > 1)
                sleep(1);
            pthread_mutex_lock(&queue_mutex);
        }
        pthread_mutex_unlock(&queue_mutex);

        // continue popping customers until the current time is less than their arrival time + willingness to wait
        pthread_mutex_lock(&queue_mutex);
        while (!ys.empty() && ys.top().arrival + ys.top().willingness_to_wait <= thread_metrics.time_elapsed)
        {
            cout_lock.lock();
            std::cout << "Thread " << pthread_self() << ": dropping customer " << ys.top().arrival << " at time " << thread_metrics.time_elapsed << std::endl;
            cout_lock.unlock();
            ys.pop();
            pthread_mutex_unlock(&queue_mutex);
            if (num_tables > 1)
                sleep(1);
            pthread_mutex_lock(&queue_mutex);
        }
        pthread_mutex_unlock(&queue_mutex);

        // if the queues are empty after dropping customers, break out of the loop
        pthread_mutex_lock(&queue_mutex);
        if (xs.empty() && ys.empty())
        {
            // pthread_mutex_unlock(&queue_mutex); // unlock the thread
            break;
        }
        pthread_mutex_unlock(&queue_mutex); // unlock the thread

        pthread_mutex_lock(&queue_mutex); // lock the thread
        while (!xs.empty() && xs.top().arrival <= thread_metrics.time_elapsed)
        {
            ys.push(xs.top());
            xs.pop();
            pthread_mutex_unlock(&queue_mutex);
            if (num_tables > 1)
                sleep(1);
            pthread_mutex_lock(&queue_mutex);
        }
        // if there are no processes to run, wait until the next one arrives
        if (ys.empty())
        {
            thread_metrics.time_elapsed = xs.top().arrival;
            break;
        }
        // run the shortest process
        Customer p = ys.top();
        ys.pop();
        pthread_mutex_unlock(&queue_mutex); // unlock the thread

        if (p.first_run == -1)
        {
            p.first_run = thread_metrics.time_elapsed;
        }
        thread_metrics.time_elapsed += p.duration;
        p.completion = thread_metrics.time_elapsed;

        pthread_mutex_lock(&completed_jobs_mutex);
        cout_lock.lock();
        std::cout << "Thread " << pthread_self() << ": completing customer " << p.arrival << " at time " << p.completion << std::endl;
        cout_lock.unlock();
        completed_jobs.push_back(p);
        pthread_mutex_unlock(&completed_jobs_mutex);
        
        pthread_mutex_lock(&queue_mutex); // lock the thread
    }
    pthread_mutex_unlock(&queue_mutex); // unlock the thread
    return;
}