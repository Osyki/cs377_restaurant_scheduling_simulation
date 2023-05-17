#include "fifo.h"
#include <unistd.h>

/**
 * Reads in a file and sets up thread to call policy.
 * @param filename: The file to read in.
 * @param num_tables: The the number of threads to spawn.
 */
FIFO::FIFO(const std::string &filename, const int &num_tables)
{
    // reads in file and sets number of threads
    read_workload(filename);
    this->xs = get_job_queue();
    this->num_tables = num_tables;
    for (int i = 0; i < num_tables; i++)
    {
        // set each thread to call run_policy
        this->threads.emplace_back(&FIFO::run_policy, this);
    }
    sleep(1);                      // sleep for 1 second to wait for all threads to initialize
    pthread_cond_broadcast(&cond); // release the condition to all threads to begin execution
}

/**
 * The scheduling policy.
 */
void FIFO::run_policy()
{
    // Stop thread from executing until all threads have been initialized.
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    // Begin scheduling policy.
    pthread_mutex_lock(&queue_mutex);
    while (!xs.empty())
    {
        pthread_mutex_unlock(&queue_mutex);

        /**
         * Get job from job queue
         */
        // Continue popping customers until the current time is less than their arrival time + willingness to wait
        pthread_mutex_lock(&queue_mutex);
        pthread_mutex_lock(&time_mutex);
        while (!xs.empty() && xs.top().arrival + xs.top().willingness_to_wait <= time_elapsed)
        {
            pthread_mutex_unlock(&time_mutex);
            xs.pop();
            pthread_mutex_unlock(&queue_mutex);
            pthread_mutex_lock(&queue_mutex);
            pthread_mutex_lock(&time_mutex);
        }
        pthread_mutex_unlock(&queue_mutex);
        pthread_mutex_unlock(&time_mutex);

        // If the queue is empty after dropping customers, break out of the loop
        pthread_mutex_lock(&queue_mutex);
        if (xs.empty())
        {
            break;
        }

        // Run the next customer
        Customer p = xs.top();
        xs.pop();
        pthread_mutex_unlock(&queue_mutex);

        /**
         * Update metrics
         */
        pthread_mutex_lock(&time_mutex);
        if (time_elapsed < p.arrival)
        {
            time_elapsed = p.arrival;
        }
        p.first_run = time_elapsed;
        // Update the time by number of tables
        time_elapsed += (p.duration % num_tables) ? (p.duration / num_tables) + 1 : (p.duration / num_tables);
        p.completion = p.first_run + p.duration;
        pthread_mutex_unlock(&time_mutex);

        /**
         * Update completed jobs
         */
        pthread_mutex_lock(&completed_jobs_mutex);
        completed_jobs.push(p);
        pthread_mutex_unlock(&completed_jobs_mutex);

        pthread_mutex_lock(&queue_mutex);
    }
    pthread_mutex_unlock(&queue_mutex);
    return;
}