#ifndef SCHEDULING_POLICY_H
#define SCHEDULING_POLICY_H

#include <list>
#include <thread>
#include <mutex>
#include <iostream>
#include "customer.h"
#include "priority_queues.h"
#include "metrics.h"

class SchedulingPolicy
{
public:
    SchedulingPolicy(); // set up job queue and read workload
    ~SchedulingPolicy();
    virtual void run_policy() = 0; // specific algorithm implementation for each policy
    void print_jobs();
    void print_metrics();
    Metrics get_metrics();

protected:
    std::list<Customer> completed_jobs;
    std::vector<std::thread> threads; // vector of threads
    int num_tables; // number of threads to run
    int time_elapsed;

    pthread_mutex_t queue_mutex;
    pthread_mutex_t completed_jobs_mutex;
    pthread_mutex_t time_mutex;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    std::mutex cout_lock{};

    void read_workload(const std::string &filename);
    pqueue_arrival get_job_queue() { return job_queue; };
    
private:
    Metrics metrics;
    pqueue_arrival job_queue;
    void join_threads();
    void calculate_metrics();
    
};

#endif