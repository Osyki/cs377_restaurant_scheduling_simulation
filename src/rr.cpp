#include "rr.h"
#include <unistd.h>

RR::RR(const std::string &filename, const int &num_tables)
{
    read_workload(filename);
    this->xs = get_job_queue();
    this->num_tables = num_tables;
    for (int i = 0; i < num_tables; i++)
    {
        this->threads.emplace_back(&RR::run_policy, this);
    }
    sleep(1); // sleep for 1 second to wait for all threads to initialize
    cout_lock.lock();
    std::cout << "\n**All threads initialized.. beginning work**\n"
              << std::endl;
    cout_lock.unlock();
    pthread_cond_broadcast(&cond);
}

// TODO: implement RR scheduling policy
void RR::run_policy()
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

        pthread_mutex_lock(&queue_mutex);
        pthread_mutex_lock(&time_mutex);
        while (!xs.empty() && xs.top().arrival + xs.top().willingness_to_wait <= time_elapsed)
        {
            pthread_mutex_unlock(&time_mutex);
            xs.pop();
            pthread_mutex_unlock(&queue_mutex);
            if (num_tables > 1)
                sleep(1);
            pthread_mutex_lock(&queue_mutex);
            pthread_mutex_lock(&time_mutex);
        }
        pthread_mutex_unlock(&queue_mutex);
        pthread_mutex_unlock(&time_mutex);

        // if the queues are empty after dropping customers, break out of the loop
        pthread_mutex_lock(&queue_mutex);
        if (xs.empty() && ys.empty())
        {
            break;
        }

        // add all processes that have arrived
        pthread_mutex_lock(&time_mutex);
        while (!xs.empty() && xs.top().arrival <= time_elapsed)
        {
            pthread_mutex_unlock(&time_mutex);
            Customer p = xs.top();
            ys.push(p);
            xs.pop();
            pthread_mutex_unlock(&queue_mutex);
            if (num_tables > 1)
                sleep(1);
            pthread_mutex_lock(&queue_mutex);
            pthread_mutex_lock(&time_mutex);
        }
        pthread_mutex_unlock(&time_mutex);

        // if there are no processes to run, wait until the next one arrives
        if (ys.empty())
        {
            pthread_mutex_lock(&time_mutex);
            time_elapsed = xs.top().arrival;
            pthread_mutex_unlock(&time_mutex);
        }
        // run the next process
        Customer p = ys.front();
        ys.pop();
        pthread_mutex_unlock(&queue_mutex);

        pthread_mutex_lock(&time_mutex);
        if (p.first_run == -1)
        {
            p.first_run = time_elapsed;
        }
        time_elapsed += 1;
        p.duration -= 1;
        if (p.duration == 0)
        {
            p.completion = time_elapsed;
            pthread_mutex_unlock(&time_mutex);
            pthread_mutex_lock(&completed_jobs_mutex);
            completed_jobs.push_back(p);
            pthread_mutex_unlock(&completed_jobs_mutex);
        }
        else
        {
            pthread_mutex_unlock(&time_mutex);
            pthread_mutex_lock(&queue_mutex);
            ys.push(p);
            pthread_mutex_unlock(&queue_mutex);
        }
        if (num_tables > 1)
            sleep(1);  
        pthread_mutex_lock(&queue_mutex);
    }
    pthread_mutex_unlock(&queue_mutex);
    return;
}