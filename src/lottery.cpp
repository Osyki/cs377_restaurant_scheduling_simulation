#include "lottery.h"
#include <unistd.h>

Lottery::Lottery(const std::string &filename, const int &num_tables)
{
    // set random seed to current time
    srand(time(NULL));
    read_workload(filename);
    this->xs = get_job_queue();
    this->num_tables = num_tables;
    for (int i = 0; i < num_tables; i++)
    {
        this->threads.emplace_back(&Lottery::run_policy, this);
    }
    sleep(1); // sleep for 1 second to wait for all threads to initialize
    cout_lock.lock();
    std::cout << "\n**All threads initialized.. beginning work**\n"
              << std::endl;
    cout_lock.unlock();
    pthread_cond_broadcast(&cond);
}

void Lottery::run_policy() {
    Metrics thread_metrics;

    pthread_mutex_lock(&mutex);
    std::cout << "Thread " << pthread_self() << ": initialized. Waiting for all threads to be ready." << std::endl;
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex); // unlocking for all other threads

    pthread_mutex_lock(&queue_mutex);
    while(!xs.empty() || !ys.empty()) {
        pthread_mutex_unlock(&queue_mutex);

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
            ys.emplace_back(p);
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
            pthread_mutex_unlock(&queue_mutex);
            pthread_mutex_lock(&time_mutex);
            time_elapsed = xs.top().arrival;
            pthread_mutex_unlock(&time_mutex);
        } else {
            // choose a random customer in queue and make sure it's arrival + willingness to wait is less than the current time
            // if it is, run the process
            // else, remove the process from the queue
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

            if (!ys.empty()) {
                // run the process
                Customer p = ys[random];
                ys.erase(ys.begin() + random);
                pthread_mutex_unlock(&queue_mutex);

                pthread_mutex_lock(&time_mutex);
                if (time_elapsed < p.arrival)
                {
                    time_elapsed = p.arrival;
                }
                p.first_run = time_elapsed;
                time_elapsed += p.duration;
                p.completion = time_elapsed;
                pthread_mutex_unlock(&time_mutex);

                pthread_mutex_lock(&completed_jobs_mutex); // lock the thread
                completed_jobs.push_back(p);
                pthread_mutex_unlock(&completed_jobs_mutex); // unlock the thread
            } else {
                pthread_mutex_unlock(&queue_mutex);
            }
        }
        
        if (num_tables > 1)
            sleep(1);                                    // short sleep to let other threads run
        pthread_mutex_lock(&queue_mutex); // lock the thread
    }
    pthread_mutex_unlock(&queue_mutex);
    return;
}