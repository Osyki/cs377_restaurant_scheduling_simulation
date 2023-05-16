#include "sjf.h"
#include <unistd.h>

/**
 * Reads in a file and sets up thread to call policy.
 * @param filename: The file to read in.
 * @param num_tables: The the number of threads to spawn.
*/
SJF::SJF(const std::string &filename, const int &num_tables)
{
    // reads in file and sets number of threads
    read_workload(filename);
    this->xs = get_job_queue();
    this->num_tables = num_tables;
    for (int i = 0; i < num_tables; i++)
    {
        // set each thread to call run_policy
        this->threads.emplace_back(&SJF::run_policy, this);
    }
    sleep(1); // sleep for 1 second to wait for all threads to initialize
    cout_lock.lock();
    std::cout << "\n**All threads initialized.. beginning work**\n" << std::endl;
    cout_lock.unlock();
    pthread_cond_broadcast(&cond); // release the condition to all threads to begin execution
}

/**
 * The scheduling policy.
*/
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

        // continue popping customers until the current time is less than their arrival time + willingness to wait
        pthread_mutex_lock(&queue_mutex);
        pthread_mutex_lock(&time_mutex);
        while (!ys.empty() && ys.top().arrival + ys.top().willingness_to_wait <= time_elapsed)
        {
            pthread_mutex_unlock(&time_mutex);
            ys.pop();
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
            // pthread_mutex_unlock(&queue_mutex); // unlock the thread
            break;
        }

        pthread_mutex_lock(&time_mutex);
        while (!xs.empty() && xs.top().arrival <= time_elapsed)
        {
            pthread_mutex_unlock(&time_mutex);
            ys.push(xs.top());
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
            pthread_mutex_unlock(&queue_mutex); // unlock the thread
            pthread_mutex_lock(&time_mutex); // unlock the thread
            time_elapsed = xs.top().arrival;
            pthread_mutex_unlock(&time_mutex); // lock the thread
        } else {
            // run the shortest process
            Customer p = ys.top();
            ys.pop();
            pthread_mutex_unlock(&queue_mutex); // unlock the thread

            pthread_mutex_lock(&time_mutex); // lock the thread
            if (p.first_run == -1)
            {
                p.first_run = time_elapsed;
            }
            time_elapsed+= p.duration;
            p.completion = time_elapsed;
            pthread_mutex_unlock(&time_mutex); // unlock the thread

            pthread_mutex_lock(&completed_jobs_mutex);
            completed_jobs.push_back(p);
            pthread_mutex_unlock(&completed_jobs_mutex);
        }
        
        if (num_tables > 1)
            sleep(1);  
        pthread_mutex_lock(&queue_mutex); // lock the thread
    }
    pthread_mutex_unlock(&queue_mutex); // unlock the thread
    return;
}