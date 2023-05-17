#include "rr.h"
#include <unistd.h>

/**
 * Reads in a file and sets up thread to call policy.
 * @param filename: The file to read in.
 * @param num_tables: The the number of threads to spawn.
 */
RR::RR(const std::string &filename, const int &num_tables)
{
    // reads in file and sets number of threads
    read_workload(filename);
    this->xs = get_job_queue();
    this->num_tables = num_tables;
    this->table_time = 0;
    for (int i = 0; i < num_tables; i++)
    {
        // set each thread to call run_policy
        this->threads.emplace_back(&RR::run_policy, this);
    }
    sleep(1);                      // sleep for 1 second to wait for all threads to initialize
    pthread_cond_broadcast(&cond); // release the condition to all threads to begin execution
}

/**
 * The scheduling policy.
 */
void RR::run_policy()
{
    // Stop thread from executing until all threads have been initialized.
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    // Begin scheduling policy.
    pthread_mutex_lock(&queue_mutex);
    while (!xs.empty() || !ys.empty())
    {
        pthread_mutex_unlock(&queue_mutex);

        /**
         * Get job from job queue
         */
        // Continue popping customers from job queue until the current time is less than their arrival time + willingness to wait
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

        // if the queues are empty after dropping customers, done with policy
        pthread_mutex_lock(&queue_mutex);
        if (xs.empty() && ys.empty())
        {
            break;
        }

        // Move customers that have arrived to the ready queue
        pthread_mutex_lock(&time_mutex);
        while (!xs.empty() && xs.top().arrival <= time_elapsed)
        {
            pthread_mutex_unlock(&time_mutex);
            Customer p = xs.top();
            ys.push(p);
            xs.pop();
            pthread_mutex_unlock(&queue_mutex);
            pthread_mutex_lock(&queue_mutex);
            pthread_mutex_lock(&time_mutex);
        }
        pthread_mutex_unlock(&time_mutex);

        // If there are no customers to run, wait until the next one arrives
        // Else get the next customer from the ready queue
        if (ys.empty())
        {
            pthread_mutex_lock(&time_mutex);
            time_elapsed = xs.top().arrival;
            pthread_mutex_unlock(&queue_mutex);
            pthread_mutex_unlock(&time_mutex);
        }
        else
        {
            // Run the next customer
            Customer p = ys.front();
            ys.pop();
            pthread_mutex_unlock(&queue_mutex);

            /**
             * Update metrics
             */
            pthread_mutex_lock(&time_mutex);
            if (p.first_run == -1)
            {
                p.first_run = time_elapsed;
            }
            // Increments universal time only if the all tables have been used
            table_time += 1;
            if (table_time >= num_tables)
            {
                table_time = 0;
                time_elapsed += 1;
            }
            p.duration -= 1;
            // If the process is done, add it to the completed jobs queue
            // Else add it back to the ready queue
            if (p.duration == 0)
            {
                /**
                 * Update completed jobs
                 */
                p.completion = time_elapsed;
                pthread_mutex_unlock(&time_mutex);
                pthread_mutex_lock(&completed_jobs_mutex);
                completed_jobs.push(p);
                pthread_mutex_unlock(&completed_jobs_mutex);
            }
            else
            {
                pthread_mutex_unlock(&time_mutex);
                pthread_mutex_lock(&queue_mutex);
                ys.push(p);
                pthread_mutex_unlock(&queue_mutex);
            }
        }
        pthread_mutex_lock(&queue_mutex);
    }
    pthread_mutex_unlock(&queue_mutex);
    return;
}