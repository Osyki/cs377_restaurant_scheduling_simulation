#ifndef SCHEDULING_POLICY_H
#define SCHEDULING_POLICY_H

#include <list>
#include <thread>
#include <iostream>
#include "customer.h"
#include "priority_queues.h"
#include "metrics.h"

class SchedulingPolicy
{
public:
    SchedulingPolicy() {}; // set up job queue and read workload
    virtual void run_policy() = 0;                                 // specific algorithm implementation for each policy
    void print_jobs();
    void print_metrics();

protected:
    std::list<Customer> completed_jobs;
    std::vector<std::thread> threads; // vector of threads
    Metrics metrics;
    int num_tables;                   // number of threads to run

    pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t completed_jobs_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    std::mutex cout_lock{};

    void read_workload(const std::string &filename);
    pqueue_arrival get_job_queue() { return job_queue; };
    
private:
    pqueue_arrival job_queue;
    void join_threads();
    void calculate_metrics();
    
};

#endif