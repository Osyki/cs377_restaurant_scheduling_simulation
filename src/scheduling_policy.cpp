#include "scheduling_policy.h"
#include <iostream>
#include <fstream>

void SchedulingPolicy::print_jobs()
{
    pqueue_arrival xs = job_queue;
    std::cout << "Processes:" << std::endl;
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
}

void SchedulingPolicy::join_threads()
{
    std::cout << "Joining threads.." << std::endl;
    for (auto &t : threads)
    {
        t.join();
    }
}

void SchedulingPolicy::print_metrics()
{
    join_threads();
    calculate_metrics();
    std::cout << "\nMetrics:" << std::endl;
    std::cout << "\tJobs Completed:          " << metrics.total_jobs_completed << "\n"
              << "\tTotal Jobs:              " << metrics.total_jobs << "\n"
              << "\tPercent Jobs Completed:  " << (double)metrics.total_jobs_completed / (double)metrics.total_jobs * 100 << "%\n"
              << "\tAverage Turnaround Time: " << metrics.avg_turnaround << "\n"
              << "\tAverage Response Time:   " << metrics.avg_response << "\n"
              << "\tRevenue Earned:          " << metrics.revenue_earned << "\n"
              << "\tRevenue Possible:        " << metrics.total_revenue << "\n"
              << "\tPercent Revenue Earned:  " << (double)metrics.revenue_earned / (double)metrics.total_revenue * 100 << "%\n"
              << "\tTotal time:              " << metrics.time_elapsed << "\n\n";
}

void SchedulingPolicy::calculate_metrics()
{
    int turnaround = 0;
    int response = 0;
    std::list<Customer> xs = completed_jobs;
    while (!xs.empty())
    {
        Customer p = xs.front();
        xs.pop_front();
        turnaround += p.completion - p.arrival;
        response += p.first_run - p.arrival;
        metrics.revenue_earned += p.revenue;
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
    metrics.avg_turnaround = (double)response / (double)completed_jobs.size();
}

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