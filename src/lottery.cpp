#include "lottery.h"
#include <unistd.h>

/**
 * Reads in a file and sets up thread to call policy.
 * @param filename: The file to read in.
 * @param num_tables: The the number of threads to spawn.
 */
Lottery::Lottery(const std::string &filename, const int &num_tables)
{
    // reads in file and sets number of threads
    srand(time(NULL)); // set random seed to current time
    read_workload(filename);
    this->xs = get_job_queue();
    this->num_tables = num_tables;
    for (int i = 0; i < num_tables; i++)
    {
        // set each thread to call run_policy
        this->threads.emplace_back(&Lottery::run_policy, this);
    }
    sleep(1); // sleep for 1 second to wait for all threads to initialize
    pthread_cond_broadcast(&cond); // release the condition to all threads to begin execution
}

/**
 * The scheduling policy.
 */
void Lottery::run_policy()
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
         * Get job from job queues
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
            ys.emplace_back(p);
            xs.pop();
            pthread_mutex_unlock(&queue_mutex);
            pthread_mutex_lock(&queue_mutex);
            pthread_mutex_lock(&time_mutex);
        }
        pthread_mutex_unlock(&time_mutex);

        // if there are no customers to serve, wait until the next one arrives
        // Else get the next customer from the ready queue
        if (ys.empty())
        {
            // Update the time to the next arrival
            pthread_mutex_lock(&time_mutex);
            time_elapsed = xs.top().arrival;
            pthread_mutex_unlock(&queue_mutex);
            pthread_mutex_unlock(&time_mutex);
        }
        else
        {
            // Choose a random customer in queue and make sure it's arrival + willingness to wait is less than the current time
            // If it is, serve the customer
            // Else, remove the customer from the queue
            int random = rand() % ys.size();
            pthread_mutex_lock(&time_mutex);
            while (!ys.empty() && ys[random].arrival + ys[random].willingness_to_wait <= time_elapsed)
            {
                pthread_mutex_unlock(&time_mutex);
                ys.erase(ys.begin() + random);
                if (!ys.empty())
                    random = rand() % ys.size();
                pthread_mutex_lock(&time_mutex);
            }
            pthread_mutex_unlock(&time_mutex);

            // If the queue is not empty, serve the customer
            // Else restart the policy
            if (!ys.empty())
            {
                // Serve the customer
                Customer p = ys[random];
                ys.erase(ys.begin() + random);
                pthread_mutex_unlock(&queue_mutex);

                /**
                 * Update metrics
                */
                pthread_mutex_lock(&time_mutex);
                if (p.first_run == -1)
                {
                    p.first_run = time_elapsed;
                }
                // Update the time by number of tables
                time_elapsed += (p.duration % num_tables) ? (p.duration / num_tables) + 1 : (p.duration / num_tables);
                p.completion = p.first_run + p.duration;
                pthread_mutex_unlock(&time_mutex);

                /**
                 * Update completed jobs
                */
                pthread_mutex_lock(&completed_jobs_mutex); // lock the thread
                completed_jobs.push(p);
                pthread_mutex_unlock(&completed_jobs_mutex); // unlock the thread
            }
            else
            {
                pthread_mutex_unlock(&queue_mutex);
            }
        }
        pthread_mutex_lock(&queue_mutex); // lock the thread
    }
    pthread_mutex_unlock(&queue_mutex);
    return;
}