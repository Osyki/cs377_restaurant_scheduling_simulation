#include "scheduling_policy.h"
#include <iostream>
#include <fstream>
#include <string.h>

/**
 * Constructor for base class. Initializes mutexes, universal time and threads_joined boolean.
 */
SchedulingPolicy::SchedulingPolicy()
{
    this->queue_mutex = PTHREAD_MUTEX_INITIALIZER;
    this->completed_jobs_mutex = PTHREAD_MUTEX_INITIALIZER;
    this->time_mutex = PTHREAD_MUTEX_INITIALIZER;
    this->mutex = PTHREAD_MUTEX_INITIALIZER;
    this->cond = PTHREAD_COND_INITIALIZER;
    time_elapsed = 0;
    threads_joined = false;
}

/**
 * Destructor for base class. Destroys mutexes.
 */
SchedulingPolicy::~SchedulingPolicy()
{
    pthread_mutex_destroy(&queue_mutex);
    pthread_mutex_destroy(&completed_jobs_mutex);
    pthread_mutex_destroy(&time_mutex);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

/**
 * Prints all jobs that are in the job queue.
 */
void SchedulingPolicy::print_jobs()
{
    if (!threads_joined)
        join_threads();
    pqueue_arrival xs = job_queue;
    std::cout << "Customers that showed up:" << std::endl;
    while (!xs.empty())
    {
        Customer p = xs.top();
        std::cout << "\tarrival=" << p.arrival
                  << ", duration=" << p.duration
                  << ", willingness_to_wait=" << p.willingness_to_wait
                  << ", revenue=" << p.revenue
                  << ", first_run=" << p.first_run
                  << ", completion=" << p.completion << std::endl;
        xs.pop();
    }
    std::cout << std::endl;
    pqueue_completion ys = completed_jobs;
    std::cout << "Customers that were helped:" << std::endl;
    while (!ys.empty())
    {
        Customer p = ys.top();
        std::cout << "\tarrival=" << p.arrival
                  << ", duration=" << p.duration
                  << ", willingness_to_wait=" << p.willingness_to_wait
                  << ", revenue=" << p.revenue
                  << ", first_run=" << p.first_run
                  << ", completion=" << p.completion << std::endl;
        ys.pop();
    }
}

/**
 * Joins all threads.
 */
void SchedulingPolicy::join_threads()
{
    for (auto &t : threads)
    {
        t.join();
    }
    threads_joined = true;
}

/**
 * Prints statistics.
 */
void SchedulingPolicy::print_metrics()
{
    if (!threads_joined)
        join_threads();
    calculate_metrics();
    std::cout << "Metrics:" << std::endl;
    std::cout << "\tJobs Completed:          " << metrics.total_jobs_completed << "\n"
              << "\tTotal Jobs:              " << metrics.total_jobs << "\n"
              << "\tPercent Jobs Completed:  " << (double)metrics.total_jobs_completed / (double)metrics.total_jobs * 100 << "%\n"
              << "\tAverage Turnaround Time: " << metrics.avg_turnaround << "\n"
              << "\tAverage Response Time:   " << metrics.avg_response << "\n"
              << "\tRevenue Earned:          " << metrics.revenue_earned << "\n"
              << "\tRevenue Possible:        " << metrics.total_revenue << "\n"
              << "\tPercent Revenue Earned:  " << (double)metrics.revenue_earned / (double)metrics.total_revenue * 100 << "%\n"
              << "\tTotal time:              " << time_elapsed << "\n\n";
}

/**
 * Calculates the metrics.
 */
void SchedulingPolicy::calculate_metrics()
{
    int turnaround = 0;
    int response = 0;
    pqueue_completion xs = completed_jobs;
    while (!xs.empty())
    {
        Customer p = xs.top();
        xs.pop();
        turnaround += p.completion - p.arrival;
        response += p.first_run - p.arrival;
        metrics.revenue_earned += p.revenue;
        time_elapsed = std::max(time_elapsed, p.completion);
    }
    pqueue_arrival ys = job_queue;
    while (!ys.empty())
    {
        Customer p = ys.top();
        ys.pop();
        metrics.total_revenue += p.revenue;
    }
    metrics.total_jobs = job_queue.size();
    metrics.total_jobs_completed = completed_jobs.size();
    metrics.avg_turnaround = (double)turnaround / (double)completed_jobs.size();
    metrics.avg_response = (double)response / (double)completed_jobs.size();
}

/**
 * Reads in the workload file.
 * @param filename: The file name to read in.
 */
void SchedulingPolicy::read_workload(const std::string &filename)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            Customer p;
            char *token = strtok(&line[0], " ");
            p.arrival = (int)strtol(token, nullptr, 10);
            token = strtok(nullptr, " ");
            p.duration = (int)strtol(token, nullptr, 10);
            token = strtok(nullptr, " ");
            p.willingness_to_wait = (int)strtol(token, nullptr, 10);
            token = strtok(nullptr, " ");
            p.revenue = (int)strtol(token, nullptr, 10);
            p.first_run = -1;
            p.completion = -1;
            this->job_queue.push(p);
        }
        file.close();
    }
    else
    {
        std::cout << "Error: Unable to open file: " << filename << std::endl;
    }
}

/**
 * Gets metrics.
 */
Metrics SchedulingPolicy::get_metrics()
{
    return metrics;
}
